#lang racket

(define directions '#(w wn n en e es s ws))

(define trail-sprite-pose-copy
  (lambda [act-name srcdir destdir]
    (define dir-count (vector-length directions))
    (define action-groups
      (let split ([ls (directory-list srcdir #:build? #false)]
                  [stca null])
        (cond [(null? ls) (reverse stca)]
              [(<= (length ls) dir-count) (split null (cons ls stca))]
              [else (split (drop ls dir-count) (cons (take ls dir-count) stca))])))

    (for ([actions (in-list action-groups)]
          [seq-idx (in-naturals 1)])
      (for ([act (in-list actions)]
            [dir (in-vector directions)])
        (define .ext (path-get-extension act))
        (define name 
          (cond [(<= (length action-groups) 1) (format "~a_~a~a" act-name dir .ext)]
                [else (format "~a_~a_~a~a" act-name dir (~r seq-idx #:min-width 3 #:pad-string "0") .ext)]))
        
        (printf "cp ~a ~a~n" act name)
        (copy-file #:exists-ok? #true
                   (build-path srcdir act)
                   (build-path destdir name))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define argv (current-command-line-arguments))

  (when (= (vector-length argv) 3)
    (trail-sprite-pose-copy (vector-ref argv 0)
                                (vector-ref argv 1)
                                (vector-ref argv 2))))
