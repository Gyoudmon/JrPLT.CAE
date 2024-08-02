#lang typed/racket/base

(require geofun/vector)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define cols : Byte 30)
(define rows : Byte 12)

(define wani : Dryland-Wani (make-dryland-wani 16))

(for ([i (in-range (add1 rows))])
  (with-dryland-wani! wani
    (jump-back)
    (jump-up i)
    (step-right cols)))

(for ([i (in-range (add1 cols))])
  (with-dryland-wani! wani
    (jump-back)
    (jump-right i)
    (step-up rows)))

(define coordinate
  (geo-freeze wani))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  coordinate)
