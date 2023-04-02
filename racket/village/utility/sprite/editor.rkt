#lang racket/gui

(require digimon/digitama/collection)
(require digimon/timer)

(require racket/class)

(require racket/path)
(require racket/string)
(require racket/draw)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define sprite%
  (class snip% (super-new)
    (init-field path raw image)

    (define/public (get-raw-width) (send raw get-width))
    (define/public (get-raw-height) (send raw get-height))
    
    (define/public (get-width) (send image get-width))
    (define/public (get-height) (send image get-height))

    (define/public (get-image) image)

    (define/override (get-extent dc x y &width &height &d &s &l &r)
      (super get-extent dc x y &width &height &d &s &l &r))

    (define/override (draw dc x y left top right bottom dx dy draw-caret?)
      (void))))

(define sprite-editor%
  (class frame% (super-new)
    (init-field sprites)

    (define master (make-custodian))
    
    (define seq-count (length sprites))
    (define &seq (box 0))

    (define cwidth 0)
    (define cheight 0)
    
    (define/override (on-superwindow-show yes?)
      (cond [(not yes?) (custodian-shutdown-all master)]
            [else (parameterize ([current-custodian master])
                    (timer-thread 256 update-sprite))]))

    (define (update-sprite-size)
      (define-values (cw ch)
        (for/fold ([cw 0] [ch 0])
                  ([s (in-list sprites)])
          (values (max cw (send s get-width))
                  (max ch (send s get-height)))))
      
      (set! cwidth cw)
      (set! cheight ch))

    (define (update-sprite thrd interval uptime)
      (send demo% refresh-now update-demo))

    (define (update-demo dc)
      (let ([seq (unbox &seq)])
        (when (< seq seq-count)
          (draw-sprite dc seq (list-ref sprites seq) 0 0)
          (set-box! &seq (remainder (+ seq 1) seq-count)))))

    (define (draw-sprite dc idx s x y)
      (define sw (send s get-width))
      (define sh (send s get-height))
      (define ix (+ x (* (- cwidth sw) 0.5)))
      (define iy (+ y (* (- cheight sh) 0.5)))

      (send dc draw-text (number->string idx) (+ x cwidth) y)
      (send dc draw-bitmap (send s get-image) ix iy))

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    (define spriteboard%
      (class pasteboard% (super-new)))

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    (define sboard (new spriteboard%))
    
    (define editor%
      (new editor-canvas% [parent this] [editor sboard]
           [style '(hide-hscroll hide-vscroll)]))

    (define demo% (new canvas% [parent this]))

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    (update-sprite-size)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define load-citizen-action
  (lambda [rootdir prefix]
    (cond [(not (directory-exists? rootdir)) null]
          [else (for/list ([action.png (in-list (directory-list rootdir #:build? #false))]
                           #:when (string-prefix? (path->string action.png) prefix))
                  (define full-action.png (build-path rootdir action.png))
                  (define raw (read-bitmap full-action.png 'unknown/alpha))
                  (make-object sprite% full-action.png raw raw))])))

(module+ main
  (define argv (current-command-line-arguments))
  (define argc (vector-length argv))

  (define action (if (>= argc 2) (vector-ref argv 1) "walk_s"))
  (define rootdir
    (cond [(>= argc 1) (vector-ref argv 0)]
          [else (simple-form-path (build-path (or (collection-root) (current-directory))
                                              'up "mascot" "trail" "Bracers" "Estelle"))]))
  
  (define editor
    (new sprite-editor%
         [sprites (load-citizen-action rootdir action)]
         [width 1200] [height 800] [parent #false]
         [label "Sprite Editor"]))
  
  (send editor show #true)
  (send editor center 'both))
