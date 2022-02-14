#lang typed/racket/base

(require bitmap)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define bitmap-car : (-> Nonnegative-Real Nonnegative-Real Nonnegative-Real Bitmap)
  (lambda [width height radius]
    (bitmap-vc-append (bitmap-rectangle width height)
                      (let ([wheel (bitmap-circle radius)]
                            [gap (bitmap-blank (- width (* radius 6)) 1)])
                        (bitmap-hc-append wheel gap wheel)))))

(define bitmap-table : (-> Nonnegative-Real Nonnegative-Real Nonnegative-Real Bitmap)
  (lambda [width height thickness]
    (define leg-width (* thickness 0.618))
    
    (bitmap-vc-append #:gapsize -1
                      (bitmap-rectangle width thickness)
                      (let ([leg (bitmap-rectangle leg-width (- height thickness))]
                            [gap (bitmap-blank (- width (* thickness 2) (* leg-width 2)) 1)])
                        (bitmap-hc-append leg gap leg)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define car-body-height : Nonnegative-Flonum 16.0)
(define car-wheel-radius : Nonnegative-Flonum 3.0)

(define force-diagram : Bitmap
  (bitmap-vc-append (bitmap-car 32.0 car-body-height car-wheel-radius)
                    (bitmap-table 128.0 64.0 8.0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  force-diagram)
