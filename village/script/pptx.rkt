#lang typed/racket/base

(require racket/file)
(require racket/string)

(require digimon/cmdopt)
(require digimon/archive)
(require digimon/format)
(require digimon/port)

(require mox/pptx)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-type CourseWare (List Path (Listof (Listof String)) Bytes))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-cmdlet-option courseware-flags #: CourseWare-Flags
  #:program 'pptx
  #:args (source . other-sources)
          
  #:banner "extract paragraphs as plain text from MS PowerPoint documents"
  #:once-each
  [[(#\a)      "append if output file exists"]
   [(#\o dest) #:=> cmdopt-string->path dest #: Path
               "write the content of courseware into ~1"]])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define read-courseware : (->* (Path) ((Option Output-Port)) (Option CourseWare))
  (lambda [course.pptx [/dev/cwout #false]]
    (fprintf /dev/stdout "reading: ~a~n" course.pptx)

    (with-handlers ([exn:fail? (λ [[e : exn:fail]] (fprintf /dev/stdout "SKIPPED: ~a~n" (exn-message e)) #false)])
      (define ps : (Listof (Listof String))
        (pptx-extract-paragraphs-from-slide
         (read-pptx-package-for-text course.pptx)))
      
      (define signature : Bytes
        (call-with-input-file* course.pptx
          sha1-bytes))

      (define cware : CourseWare (list course.pptx ps signature))

      (when (and /dev/cwout)
        (write-courseware cware /dev/cwout)
        (flush-output /dev/cwout))
      
      cware)))

(define read-coursewares : (->* ((Listof Path)) ((Option Output-Port)) (Listof CourseWare))
  (lambda [course.pptx [/dev/cwout #false]]
    (for/fold ([cwares : (Listof CourseWare) null])
              ([source (in-list course.pptx)])
      (cond [(file-exists? source)
             (cond [(not (regexp-match? #px"[.]pptx$" source)) cwares]
                   [else (let ([cware (read-courseware source /dev/cwout)])
                           (cond [(not cware) cwares]
                                 [else (append cwares (list cware))]))])]
            [(directory-exists? source)
             (cond [(regexp-match? #px"/[.][^/]$" source) cwares]
                   [else (let ([subfiles (directory-list source #:build? #true)])
                           (append cwares (read-coursewares subfiles /dev/cwout)))])]
            [else cwares]))))

(define write-courseware : (->* (CourseWare) (Output-Port) Void)
  (lambda [cware [/dev/cwout (current-output-port)]]
    (fprintf /dev/stdout "writing: ~a~n" (car cware))
    
    (displayln (car cware) /dev/cwout)
    (display (bytes->hexstring (caddr cware)) /dev/cwout)

    (if (pair? (cadr cware))
        (for ([ps (in-list (cadr cware))])
          (newline /dev/cwout)
          
          (for ([p (in-list ps)])
            (when (non-empty-string? p)
              (displayln p /dev/cwout))))
        (newline /dev/cwout))))
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define-values (options λargv) (parse-courseware-flags (current-command-line-arguments)))
  
  (when (courseware-flags-help? options)
    (display-courseware-flags #:exit 0))

  (parameterize ([default-archive-entry-progress-handler (make-archive-entry-terminal-gauge #:final-char #\space #:overlay-name? #true)]
                 [default-archive-progress-handler (make-archive-terminal-gauge #:at (cons 1 128))]
                 [current-custodian (make-custodian)])
    (dynamic-wind
     (λ [] (when (courseware-flags-dest options)
             (define o (courseware-flags-dest options))
             (make-parent-directory* o)
             (current-output-port (open-output-file #:exists (if (courseware-flags-a options)
                                                                 'append
                                                                 'truncate/replace)
                                                    o))))

     (λ [] (void
            (let-values ([(source others) (λargv)])
              (read-coursewares (map path->complete-path (cons source others))
                                (current-output-port)))))
    
     (λ [] (custodian-shutdown-all (current-custodian))))))
