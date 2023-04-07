#lang racket/base

(provide (all-defined-out))

(require racket/gui/base)

(require racket/class)
(require racket/flonum)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define the-dc (make-object bitmap-dc% (make-bitmap 1 1 #true #:backing-scale 2.0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define digicore-snip%
  (class snip% (super-new)
    (send* this
      (set-snipclass (new snip-class%))
      (set-flags (cons 'handles-all-mouse-events (send this get-flags))))
    
    (inherit get-admin get-extent)

    (define enabled #true)
    
    (define/public (enable ?) (set! enabled ?))
    (define/public (enabled?) enabled)
    (define/public (on-insert zone) (void))
    (define/public (on-delete zone) (void))
    (define/public (on-select ? event) #false)
    (define/public (on-hover ? event) #false)
    (define/public (on-active ? event) #false)
    (define/public (get-tooltip dc x y editor-x editor-y mouse) #false)
    
    (define/public (selected?)
      (define master (get-admin))
      (and master
           (let ([zone (send master get-editor)])
             (and zone
                  (send zone is-selected? this)))))

    (define/public (refresh-now)
      (define master (get-admin))
      (define editor (and master (send master get-editor)))
      (unless (not editor)
        (define-values (w h) (get-size))
        (send editor needs-update this 0 0 w h)))
    
    (define/public (get-zone-size) ; insets have already been subtracted
      (define master (get-admin))
      (cond [(not master) (values 0.0 0.0)]
            [else (let ([&width (box 0.0)]
                        [&height (box 0.0)])
                    (send master get-view-size &width &height)
                    (values (real->double-flonum (unbox &width))
                            (real->double-flonum (unbox &height))))]))

    (define/public (get-size)
      ;;; NOTE
      ; racket/snip has its own caching strategy,
      ; getting size through editor is therefore better than directly invoking (get-extent)  
      (define master (get-admin))
      (define editor (and master (send master get-editor)))
      (if (not editor)
          (let ([&width (box 0.0)]
                [&height (box 0.0)])
            (get-extent the-dc 0.0 0.0 &width &height)
            (values (real->double-flonum (unbox &width))
                    (real->double-flonum (unbox &height))))
          (let ([dc (or (and master (send master get-dc)) the-dc)]
                [&x (box 0.0)]
                [&y (box 0.0)])
            ;;; WARNING: Client may have to call (resized) first
            (send editor get-snip-location this &x &y #false)
            (define-values (x y) (values (unbox &x) (unbox &y)))
            (send editor get-snip-location this &x &y #true)
            (values (flabs (real->double-flonum (- (unbox &x) x)))
                    (flabs (real->double-flonum (- (unbox &y) y)))))))
    
    (define/override (copy)
      (raise (make-exn:fail:unsupported "this feature is disabled!" (current-continuation-marks))))
    
    (define/override (write /dev/edtout)
      (raise (make-exn:fail:unsupported "this feature has not been implemented yet!" (current-continuation-marks))))))

(define digicore-zone%
  (class pasteboard% (super-new)
    (send* this
      (set-selection-visible #false)
      (auto-wrap #true))

    (inherit begin-edit-sequence end-edit-sequence get-dc get-admin)
    (inherit insert find-first-snip find-next-selected-snip get-snip-location)
    (inherit set-selected remove-selected is-selected? no-selected set-caret-owner)
    (inherit dc-location-to-editor-location #| <==> |# global-to-local)
    (inherit editor-location-to-dc-location #| <==> |# local-to-global)

    (define &hover (box #false))
    
    (define/augment (can-interactive-move? mouse) (inner #false can-interactive-move? mouse))
    (define/augment (can-interactive-resize? mouse) (inner #false can-interactive-resize? mouse))
    (define/augment (can-load-file? filename format) (inner #false can-load-file? filename format))
    (define/augment (can-save-file? filename format) (inner #false can-save-file? filename format))

    (define/public (get-snip-rectangle snip #:global? [dc-coordinate? #false])
      (define &x (box 0.0))
      (define &y (box 0.0))
      (define &width (box 0.0))
      (define &height (box 0.0))
      (get-snip-location snip &x &y) ; TODO: why must call (get-extent)?
      (send snip get-extent (or (get-dc) the-dc) (unbox &x) (unbox &y) &width &height)
      (when dc-coordinate? (local-to-global &x &y))
      (values (real->double-flonum (unbox &x)) (real->double-flonum (unbox &y))
              (real->double-flonum (unbox &width)) (real->double-flonum (unbox &height))))

    (define/public (get-zone-rectangle #:global? [dc-coordinate? #false])
      (define &x0 (box +inf.0))
      (define &y0 (box +inf.0))
      (define &xn (box 0.0))
      (define &yn (box 0.0))
      (let fold-snip ([this-snip (find-first-snip)])
        (if (not this-snip)
            (let-values ([(w h) (values (- (unbox &xn) (unbox &x0)) (- (unbox &yn) (unbox &y0)))])
              (when dc-coordinate? (local-to-global &x0 &y0))
              (values (real->double-flonum (unbox &x0)) (real->double-flonum (unbox &y0))
                      (flmax (real->double-flonum w) 0.0) (flmax (real->double-flonum h) 0.0)))
            (let-values ([(x y w h) (get-snip-rectangle this-snip #:global? #false)])
              (when (< x (unbox &x0)) (set-box! &x0 x))
              (when (< y (unbox &y0)) (set-box! &y0 y))
              (when (> (fl+ x w) (unbox &xn)) (set-box! &xn (fl+ x w)))
              (when (> (fl+ y h) (unbox &yn)) (set-box! &yn (fl+ y h)))
              (fold-snip (send this-snip next))))))
    
    (define/public (get-zone-view)
      (define &x (box 0.0))
      (define &y (box 0.0))
      (define &width (box 0.0))
      (define &height (box 0.0))
      (define master (get-admin))
      (cond [(not master) (values 0.0 0.0 0.0 0.0)]
            [else (let ([_ (send master get-view &x &y &width &height #false)])
                    (values (real->double-flonum (unbox &x)) (real->double-flonum (unbox &y))
                            (flmax (real->double-flonum (unbox &width)) 0.0)
                            (flmax (real->double-flonum (unbox &height)) 0.0)))]))

    (define/public (find-last-snip)
      (let try ([this-snip (find-first-snip)])
        (define next-snip (and this-snip (send this-snip next)))
        (if (not next-snip) this-snip (try next-snip))))

    (define/public (do-interactive-operation snip operation event)
      (and (is-a? snip digicore-snip%)
           (case operation
             [(hover)    (send snip on-hover  #true  event)]
             [(active)   (send snip on-active #true  event)]
             [(deactive) (send snip on-active #false event)]
             [(goodbye)  (send snip on-hover  #false event)]
             [(select)   (send snip on-select #true  event)]
             [(deselect) (send snip on-select #false event)]
             [(insert)   (send snip on-insert this   event)]
             [(delete)   (send snip on-delete this   event)]
             [else #false])))

    (define/public (ordered-insert snip<? snip [x 0] [y 0] #:unique? [unique? #true])
      (let insert/compare-with ([this-snip (find-first-snip)])
        (cond [(not this-snip) (insert snip x y)]
              [else (let ([uuid (snip<? snip this-snip)])
                      (cond [(eq? uuid #true) (insert snip this-snip x y)]
                            [(and uuid unique?) (error 'ordered-insert "snip has already existed: ~a" uuid)]
                            [else (insert/compare-with (send this-snip next))]))])))

    (define/public (enable ?)
      (let try-next ([child (find-first-snip)])
        (unless (not child)
          (send child enable ?)
          (try-next (send child next)))))

    (define/override (find-snip editor-x editor-y [after #false])
      (let try ([maybe-snip (or after (find-first-snip))])
        (and maybe-snip
             (let-values ([(x y w h) (get-snip-rectangle maybe-snip #:global? #false)])
               (cond [(and (<= x editor-x (+ x w)) (<= y editor-y (+ y h))) maybe-snip]
                     [else (try (send maybe-snip next))])))))
  
    (define/override (adjust-cursor mouse)
      (define maybe-cursor
        (when (get-admin)
          (define &editor-x (box #| initiated with global x |# (send mouse get-x)))
          (define &editor-y (box #| initiated with global y |# (send mouse get-y)))  
          (global-to-local &editor-x &editor-y)
          (define maybe-snip (find-snip (unbox &editor-x) (unbox &editor-y)))
          (unless (not maybe-snip)
            (get-snip-location maybe-snip &editor-x &editor-y)
            (define-values (dc-x dc-y) (editor-location-to-dc-location (unbox &editor-x) (unbox &editor-y)))
            (send maybe-snip adjust-cursor (or (get-dc) the-dc) dc-x dc-y (unbox &editor-x) (unbox &editor-y) mouse))))
      (if (void? maybe-cursor) (super adjust-cursor mouse) maybe-cursor))
    
    (define/override (on-event mouse)
      (super on-event mouse)
      (define &editor-x (box #| initiated with global x |# (send mouse get-x)))
      (define &editor-y (box #| initiated with global y |# (send mouse get-y)))  
      (global-to-local &editor-x &editor-y)
      (define maybe-snip (find-snip (unbox &editor-x) (unbox &editor-y)))
      (let ([hovered (unbox &hover)])
        (when (and hovered (not (eq? hovered maybe-snip)))
          (do-interactive-operation hovered 'goodbye mouse))
        (set-box! &hover maybe-snip)
        (when (and maybe-snip (send mouse moving?) (not (send mouse get-left-down)))
          (do-interactive-operation maybe-snip 'hover mouse))))

    (define/override (on-default-event mouse)
      (define maybe-snip
        (let-values ([(editor-x editor-y) (dc-location-to-editor-location (send mouse get-x) (send mouse get-y))])
          (find-snip editor-x editor-y)))
      (when (and maybe-snip)
        (cond [(or (send mouse button-down? 'left)
                   (and (send mouse moving?)
                        (send mouse get-left-down)))
               (void (do-interactive-operation maybe-snip 'active mouse))]
              [(send mouse button-up? 'left)
               (set-selected maybe-snip)
               (void (do-interactive-operation maybe-snip 'deactive mouse))])))

    (define/override (on-default-char keyboard)
      (unless (memq (send keyboard get-key-code)
                    '(left right up down #\backspace #\rubout))
        (super on-default-char keyboard)))
    
    (define/augment (on-insert snip before x y)
      (begin-edit-sequence)
      (inner (void) on-insert snip before x y))
    
    (define/augment (after-insert snip before x y)
      (inner (void) after-insert snip before x y)
      (when (is-a? snip digicore-snip%) (send snip on-insert this))
      (end-edit-sequence))
    
    (define/augment (on-delete snip)
      (begin-edit-sequence)
      (inner (void) on-delete snip))
    
    (define/augment (after-delete snip)
      (when (is-a? snip digicore-snip%) (send snip on-delete this))
      (inner (void) after-delete snip)
      (end-edit-sequence))))
