#lang typed/racket/base

(require geofun/vector)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define cols : Byte 30)
(define rows : Byte 12)

(define gomamon : Gomamon (make-gomamon 16))

(for ([i (in-range (add1 rows))])
  (with-gomamon! gomamon
    (jump-back)
    (jump-up i)
    (move-right cols)))

(for ([i (in-range (add1 cols))])
  (with-gomamon! gomamon
    (jump-back)
    (jump-right i)
    (move-up rows)))

(define coordinate
  (geo-freeze gomamon))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  coordinate)
