#lang typed/racket/base

(require bitmap)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define math-font (desc-font #:size 14.0 #:family 'math))
(define cs-line (desc-stroke #:color 'gray #:dash 'short-dash))
(define arrow-color 'lightsteelblue)
(define ch (font-metrics-ref math-font 'ch))
(define axis-length 256.0)
(define ar 4.0)

(define (cs-xyline [x : Real] [y : Real]) : Bitmap
  (bitmap-rb-superimpose (bitmap-hline x 0.5 #:stroke cs-line) (bitmap-vline 0.5 y #:stroke cs-line)))

(define (cs-dot [x : Real] [y : Real] [c : Color 'black]) : Bitmap
  (bitmap-pin* 1.0 1.0 0.5 0.5 (cs-xyline x y)
               (bitmap-circle 2.0 #:stroke #false #:fill c)))

(define (cs-rect [x : Real] [y : Real] [w : Real] [h : Real] [c : Color 'black]) : Bitmap
  (bitmap-pin* 1.0 1.0 0.0 0.0 (cs-xyline x y)
               (bitmap-ct-superimpose (bitmap-rc-superimpose (bitmap-rectangle w h #:stroke c) (bitmap-text "h " math-font))
                                      (bitmap-text "w " math-font))))

(define (cs-circle [x : Real] [y : Real] [r : Real] [c : Color 'black]) : Bitmap
  (bitmap-pin-over (cs-xyline x y)
                   x y
                   (bitmap-hc-append (bitmap-rc-superimpose (bitmap-circle r #:stroke c) (bitmap-arrow ar (- r ar) #:fill arrow-color))
                                     (bitmap-text 'R math-font))
                   r r))

(define (cs-ellipse [x : Real] [y : Real] [a : Real] [b : Real] [c : Color 'black]) : Bitmap
  (bitmap-pin-over (cs-xyline x y)
                   x y
                   (bitmap-vc-append
                    (bitmap-hc-append
                     (bitmap-cb-superimpose (bitmap-rc-superimpose (bitmap-ellipse (* a 2) (* b 2) #:stroke c)
                                                                   (bitmap-arrow ar (- a ar) #:fill arrow-color))
                                            (bitmap-arrow ar (- b ar) pi/2 #:fill arrow-color))
                     (bitmap-text 'a math-font))
                    (bitmap-text 'b math-font))
                   a b))

(define (cartesian-pin [cs : Bitmap] [shapes : (Listof Bitmap)]) : Bitmap
  (define pin-pt (+ (* ch 2) 2+2i))
  (for/fold ([bmp : Bitmap cs])
            ([shape (in-list shapes)])
    (bitmap-pin-over bmp (real-part pin-pt) (imag-part pin-pt) shape)))

(define shape-demo
  (bitmap-scale
   (cartesian-pin
    (bitmap-hb-append #:gapsize ch
                      (bitmap-text "y " math-font)
                      (bitmap-lt-superimpose
                       (bitmap-vr-append (bitmap-arrow ar axis-length) (bitmap-text "x" math-font))
                       (bitmap-arrow ar axis-length pi/2)))
    
    (list (cs-dot 30 40 'seagreen)
          (cs-rect 60 80 60 42 'forestgreen)
          (cs-circle 200 140 24 'royalblue)
          (cs-ellipse 140 200 32 24 'crimson)))
   2.0))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  shape-demo)
