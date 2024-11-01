#lang typed/racket/base

(require racket/string)

(require digimon/cmdopt)

(require "schema.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-cmdlet-option schema-flags #: Schema-Flags
  #:program 'schema
  #:args [source]
          
  #:banner "import the raw info into a sqlite database"
  #:once-each
  [[(reset) "clear the database file if exists"] 
   [(#\o dest) #:=> cmdopt-string->path dest #: Path
               "specific the path ~1 of the dateabase"]])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define course-info->schema : (-> Input-Port Connection Void)
  (lambda [/dev/cwin schema]
    (courseware:create schema #:if-not-exists? #true)

    (for/fold ([record : (U False String (List String String (Listof String))) #false])
              ([line (in-lines /dev/cwin)])
      (cond [(regexp-match? #px"^/Volumes/VirtualServer" line)
             (with-handlers ([exn:fail? (λ [[e : exn:fail]] (displayln (exn-message e) (current-error-port)))])
               (when (list? record)
                 (define row (courseware:seek schema #:where (list "sha1 = ~a" (cadr record))))
                 (if (not row)
                     (let ([content (string-join (caddr record) "\n")])
                       (printf "inserting ~a [~a]~n" (car record) (cadr record))
                       (courseware:insert schema (make-courseware #:path (car record) #:sha1 (cadr record) #:content content)))
                     (let* ([duplicates (courseware-duplicates row)]
                            [duplicates (if (not duplicates) (car record) (string-append duplicates "\n" (car record)))])
                       (printf "updating ~a [~a]~n" (car record) (cadr record))
                       (courseware:update schema (remake-courseware row #:duplicates duplicates))))))
             line]
            [(string? record) (list record line null)]
            [(list? record) (list (car record) (cadr record) (cons line (caddr record)))]
            [else record]))

    (void)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define-values (options λargv) (parse-schema-flags (current-command-line-arguments)))
  
  (when (schema-flags-help? options)
    (display-schema-flags #:exit 0))
  
  (define schema : Connection
    (if (schema-flags-dest options)
        (let ([db (schema-flags-dest options)])
          (when (schema-flags-reset options)
            (when (file-exists? db)
              (delete-file db)))
          (sqlite3-create-database-if-not-exists db)
          (sqlite3-connect #:database db))
        (sqlite3-connect #:database 'memory)))

  (unless (member "courseware" (list-tables schema))
    (with-handlers ([exn:fail? (λ [[e : exn:fail]] (displayln (exn-message e) (current-error-port)))])
      (call-with-input-file* (path->complete-path (λargv))
        (lambda [[/dev/cwin : Input-Port]]
          (course-info->schema /dev/cwin schema)))))

  (?courseware:count schema)
  (disconnect schema))
