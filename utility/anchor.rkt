#lang typed/racket/base

(require bitmap)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define anchor-font (desc-font #:size 24))

(define anchor-demo
  (for/fold ([base : Bitmap (bitmap-solid 'palegreen 256)])
            ([anchor (in-list '(LT CT RT LC CC RC LB CB RB))])
    (let ([label (bitmap-text anchor anchor-font #:color 'crimson)])
      (case anchor
        [(CT) (bitmap-ct-superimpose base label)]
        [(RT) (bitmap-rt-superimpose base label)]
        [(LC) (bitmap-lc-superimpose base label)]
        [(CC) (bitmap-cc-superimpose base label)]
        [(RC) (bitmap-rc-superimpose base label)]
        [(LB) (bitmap-lb-superimpose base label)]
        [(CB) (bitmap-cb-superimpose base label)]
        [(RB) (bitmap-rb-superimpose base label)]
        [else (bitmap-lt-superimpose base label)]))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  anchor-demo)
