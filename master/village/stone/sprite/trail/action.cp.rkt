#lang racket

(define trail-sprite-action-copy
  (lambda [act-name srcdir destdir]
    (for ([seq (in-naturals 1)]
          [img (in-list (directory-list srcdir #:build? #false))])
      (define .ext (path-get-extension img))
      (define name (format "~a_~a~a" act-name (~r seq #:min-width 3 #:pad-string "0") .ext))
      (printf "cp ~a ~a~n" img name)
      (copy-file #:exists-ok? #true
                 (build-path srcdir img)
                 (build-path destdir name)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define argv (current-command-line-arguments))

  (when (= (vector-length argv) 3)
    (trail-sprite-action-copy (vector-ref argv 0)
                              (vector-ref argv 1)
                              (vector-ref argv 2))))
