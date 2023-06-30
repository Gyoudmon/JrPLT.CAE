#lang typed/racket/base

(provide (all-defined-out))

(require schema)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-schema CourseWares
  (define-table courseware #:as CourseWare #:with uuid
    ([uuid       : Integer #:default (pk64:timestamp)]
     [sha1       : String  #:not-null #:unique]
     [path       : String  #:not-null]
     [domain     : Symbol]
     [subdomain  : Symbol  #:default #false]
     [topic      : String  #:default #false]
     [content    : String  #:default #false]
     [duplicates : String  #:default #false])))
