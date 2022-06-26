#lang typed/racket

(require bitmap)

(define make-2d-array : (->* (Positive-Byte Positive-Byte) ((Listof (List Byte Byte Color)) #:size Real #:gapsize Nonnegative-Real) Bitmap)
  (lambda [row col [cells null] #:size [size 24] #:gapsize [gap 8.0]]
    (bitmap-table* (cons (cons (bitmap-blank)
                               (for/list : (Listof Bitmap) ([c (in-range col)])
                                 (bitmap-text (format "第~a列" c) #:color 'green)))
                         (for/list : (Listof (Listof Bitmap)) ([r (in-range row)])
                           (cons (bitmap-text (format "第~a行" r) #:color 'blue)
                                 (for/list : (Listof Bitmap) ([c (in-range col)])
                                   (define specell (assq r cells))
                                   (if (and specell (eq? (cadr specell) c))
                                       (bitmap-square size #:fill (caddr specell))
                                       (bitmap-square size))))))
                   'cc 'cc gap gap)))

(define 2d-array
  (make-2d-array 8 8
                 '((2 3 cyan)
                   (4 4 purple))))

(module+ main
  2d-array)
