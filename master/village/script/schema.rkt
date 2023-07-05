#lang typed/racket/base

(provide (all-defined-out))
(provide (all-from-out schema))
(provide (all-from-out schema/sqlite3))

(require schema)
(require schema/sqlite3)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-schema CourseWares
  (define-table courseware #:as CourseWare #:with uuid
    ([uuid       : Integer #:default (pk64:timestamp)]
     [sha1       : String  #:not-null #:unique]
     [path       : String  #:not-null]
     [categories : String]
     [content    : String  #:default #false]
     [duplicates : String  #:default #false])))
