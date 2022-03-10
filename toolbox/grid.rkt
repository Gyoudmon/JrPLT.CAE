#lang typed/racket/base

(require bitmap)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define cols : Byte 16)

(define wani : Dryland-Wani (make-dryland-wani 16))

(for ([i (in-range (add1 cols))])
  (with-dryland-wani! wani
    (jump-back)
    (jump-up i)
    (step-right cols)
    
    (jump-back)
    (jump-right i)
    (step-up cols)))

(define coordinate
  (bitmap-track wani))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  coordinate)
