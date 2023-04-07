#lang racket/gui

(require digimon/digitama/collection)
(require digimon/timer)

(require racket/class)

(require racket/path)
(require racket/string)
(require racket/draw)

(require "digicore.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(struct sprite-info
  (cwidth cheight seq size)
  #:transparent
  #:mutable)

(define calculate-sprites-size
  (lambda [sprites]
    (for/fold ([cw 0] [ch 0])
              ([s (in-list sprites)])
      (values (max cw (send s get-width))
              (max ch (send s get-height))))))

(define make-sprite-info
  (lambda [sprites]
    (define-values (w h) (calculate-sprites-size sprites))
    (sprite-info w h 0 (length sprites))))

(define sprite-info-update-size!
  (lambda [sinfo sprites]
    (define-values (w h) (calculate-sprites-size sprites))

    (set-sprite-info-cwidth! sinfo w)
    (set-sprite-info-cheight! sinfo h)))

(define sprite-info-step!
  (lambda [sinfo]
    (define seq (sprite-info-seq sinfo))
    (define seq++ (add1 seq))

    (if (>= seq++ (sprite-info-size sinfo))
        (set-sprite-info-seq! sinfo 0)
        (set-sprite-info-seq! sinfo seq++))
    
    seq))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define sprite%
  (class digicore-snip% (super-new)
    (init-field path raw image)

    (define gray-brush (make-object brush% "Gray"))
    (define no-brush (make-object brush% "Gray" 'transparent))
    (define gray-pen (make-object pen% "Gray" 1))
    (define blue-pen (make-object pen% "SkyBlue" 1))
    (define no-pen (make-object pen% "Black" 1 'transparent))

    (define/public (get-raw-width) (send raw get-width))
    (define/public (get-raw-height) (send raw get-height))
    
    (define/public (get-width) (send image get-width))
    (define/public (get-height) (send image get-height))

    (define/public (get-image) image)

    (define/override (get-extent dc x y &width &height &d &s &l &r)
      (super get-extent dc x y &width &height &d &s &l &r))

    (define/override (draw dc x y left top right bottom dx dy draw-caret?)
      (define grid-size 16)
      (define-values (w h) (values (get-width) (get-height)))

      (send dc set-clipping-rect x y w h)
      (send dc set-brush gray-brush)
      (send dc set-pen no-pen)

      (let draw-grid-row ([dy 0])
        (when (< dy h)
          (let draw-grid ([dx (if (even? (quotient dy grid-size)) 0 grid-size)])
            (when (< dx w)
              (send dc draw-rectangle (+ x dx) (+ y dy) grid-size grid-size)
              (draw-grid (+ dx grid-size grid-size))))
          (draw-grid-row (+ dy grid-size))))

      (send dc set-brush no-brush)
      (send dc set-pen (if (send this selected?) blue-pen gray-pen))
      (send dc draw-rectangle x y w h)
      
      (send dc draw-bitmap image x y))

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    (send this set-snipclass (new snip-class%))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define sprite-editor%
  (class frame% (super-new)
    (init-field sprites)
    (init-field [grid-width 64] [grid-height 128] [grid-gap 4])

    (define master (make-custodian))
    
    (define infos
      (for/hasheq ([(key value) (in-hash sprites)])
        (values key (make-sprite-info value))))
    
    (define/override (on-superwindow-show yes?)
      (cond [(not yes?) (custodian-shutdown-all master)]
            [else (parameterize ([current-custodian master])
                    (timer-thread 128 update-sprite))]))

    (define (update-sprite thrd interval uptime)
      (send demo% refresh-now update-demo))

    (define (update-demo dc)
      (define s (send demo% min-height))
      (for ([(key info) (in-hash infos)]
            [pos (in-naturals)])
        (define idx (sprite-info-step! info))
        (define snip (list-ref (hash-ref sprites key) idx))
        (draw-demo-sprite dc idx snip (* s pos) 0
                          (sprite-info-cwidth info)
                          (sprite-info-cheight info))))
    
    (define (draw-demo-sprite dc idx snip x y cwidth cheight)
      (define S (send demo% min-height))
      (define sw (send snip get-width))
      (define sh (send snip get-height))
      (define bx (+ x (* (- S cwidth) 0.5)))
      (define by (+ y (* (- S cheight) 0.5)))
      (define ix (+ bx (* (- cwidth sw) 0.5)))
      (define iy (+ by (* (- cheight sh) 0.5)))
      
      (send dc draw-rectangle bx by cwidth cheight)
      (send dc draw-text (number->string idx) x y)
      (send dc draw-bitmap (send snip get-image) ix iy))

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    (define spriteboard%
      (class digicore-zone% (super-new)))

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    (define sboard1 (new spriteboard%))
    (define sboard2 (new spriteboard%))

    (define tool-panel (new horizontal-pane% [parent this] [min-height 96] [stretchable-height #false]))
    (define editor-panel (new horizontal-pane% [parent this]))

    (define demo% (new canvas% [parent tool-panel] [min-height 96] [stretchable-height #false]))
    
    (define editor1%
      (new editor-canvas% [parent editor-panel] [editor sboard1]
           [style '(hide-hscroll hide-vscroll)]))

    (define editor2%
      (new editor-canvas% [parent editor-panel] [editor sboard2]
           [style '(hide-hscroll hide-vscroll)]))

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    (for ([(key costumes) (in-hash sprites)]
          [idx (in-naturals)])
      (for ([sprite (in-list costumes)]
            [jdx (in-naturals)])
        (define-values (board x y)
          (if (even? idx)
              (values sboard1 (* jdx (+ grid-width grid-gap)) (* (/ idx 2) (+ grid-height grid-gap)))
              (values sboard2 (* jdx (+ grid-width grid-gap)) (* (/ (- idx 1) 2) (+ grid-height grid-gap)))))
        (define w (send sprite get-width))
        (define h (send sprite get-height))
        (send board insert sprite
              (+ x (quotient (- grid-width w) 2))
              (+ y (quotient (- grid-height h) 2)))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define load-citizen-action
  (lambda [rootdir type]
    (define px:name #px"(\\D+)(\\d+)?[.]png")

    (cond [(not (directory-exists? rootdir)) (hasheq)]
          [else (for/fold ([sprites (hasheq)])
                          ([action.png (in-list (directory-list rootdir #:build? #false))]
                           #:when (string-prefix? (path->string action.png) type))
                  (define tokens (regexp-match px:name (path->string action.png)))
                  (cond [(not tokens) sprites]
                        [else (let* ([full-action.png (build-path rootdir action.png)]
                                     [raw (read-bitmap full-action.png 'unknown/alpha)])
                                (define-values (prefix subseq) (values (string->symbol (string-trim (cadr tokens) "_")) (caddr tokens)))
                                (hash-set sprites prefix
                                          (append (hash-ref sprites prefix list)
                                                  (list (make-object sprite% full-action.png raw raw)))))]))])))

(module+ main
  (define argv (current-command-line-arguments))
  (define argc (vector-length argv))

  (define action (if (>= argc 2) (vector-ref argv 1) "walk"))
  (define rootdir
    (cond [(>= argc 1) (vector-ref argv 0)]
          [else (simple-form-path (build-path (or (collection-root) (current-directory))
                                              'up "mascot" "trail" "Bracers" "Estelle"))]))
  
  (define editor
    (new sprite-editor%
         [sprites (load-citizen-action rootdir action)]
         [width 1200] [height 900] [parent #false]
         [label "Sprite Editor"]))
  
  (send editor show #true)
  (send editor maximize #true)
  #;(send editor center 'both))
