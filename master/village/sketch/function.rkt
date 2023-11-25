#lang racket

(require plot)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (S r)
  (* pi r r))

(define (wave A ω φ [k 0])
  (define (Asinωx+φ x)
    (+ (* A (sin (+ (* ω x) φ)))
       k))
  Asinωx+φ)

(define (V r h)
  (* 1/3 pi r r h))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(plot #:x-min -8 #:x-max 8
      #:y-min -4 #:y-max 4
      (list (function S #:color "orange")
            (function (wave 0.5 2.0 1.0) #:color "red")
            (function (wave 1.0 2.0 1.0) #:color "green")
            (x-axis)
            (y-axis)))

(plot3d (surface3d V 0 16 0 16)
        #:altitude 25)
