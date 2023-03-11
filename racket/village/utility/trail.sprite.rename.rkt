#lang racket

(define directions '#(w wn n en e es s ws))

(define trail-sprite-moving-rename
  (lambda [act-name rootdir]
    (define dir-count (vector-length directions))
    (define action-groups
      (let split ([ls (directory-list rootdir #:build? #false)]
                  [stca null])
        (cond [(null? ls) (reverse stca)]
              [(<= (length ls) dir-count) (split null (cons ls stca))]
              [else (split (drop ls dir-count) (cons (take ls dir-count) stca))])))

    (for ([actions (in-list action-groups)]
          [seq-idx (in-naturals 1)])
      (for ([act (in-list actions)]
            [dir (in-vector directions)])
        (define .ext (path-get-extension act))
        (define name (format "~a_~a_~a~a" act-name dir (~r seq-idx #:min-width 3 #:pad-string "0") .ext))
        (printf "mv ~a ~a~n" act name)
        (rename-file-or-directory (build-path rootdir act)
                                  (build-path rootdir name))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define argv (current-command-line-arguments))

  (when (> (vector-length argv) 0)
    (for ([rootdir (in-vector argv 1)])
      (trail-sprite-moving-rename (vector-ref argv 0) rootdir))))
