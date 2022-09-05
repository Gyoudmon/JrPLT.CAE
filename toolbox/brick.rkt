#lang racket

(require bitmap)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (brick-number layer)
  (cond [(<= layer 0) 0]
        [(= layer 1) 1]
        [else (+ layer (brick-number (sub1 layer)))]))

(define brick-heap
  (let ([block (bitmap-rectangle 32 16 #:border 'orangered #:fill 'firebrick)])
    (lambda [layer]
      (cond [(<= layer 0) (bitmap-blank)]
            [else (bitmap-vc-append*
                   (for/list ([l (in-range (add1 layer))])
                     (bitmap-hc-append* (make-list (brick-number l) block))))]))))

(define bricks (brick-heap 5))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  bricks)
