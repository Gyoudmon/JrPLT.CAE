#lang typed/racket/base

(require racket/file)

(require digimon/cmdopt)
(require digimon/port)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-type CourseWare (List Path (Listof (Listof String)) Bytes))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-cmdlet-option courseware-flags #: CourseWare-Flags
  #:program 'filter
  #:args [source]
          
  #:banner "clear content of the raw source file"
  #:once-each
  [[(#\o dest) #:=> cmdopt-string->path dest #: Path
               "write the cleared content of courseware into ~1"]])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define courseware-write-line : (-> Output-Port String Void)
  (lambda [/dev/cwout line]
    (displayln line /dev/cwout)
    (flush-output /dev/cwout)))

(define courseware-string-okay? : (-> String Boolean)
  (lambda [line]
    (cond [(member line (list "洛克白STEM教育研学中心")) #false]
          [(regexp-match? #px"^\\s*[Tt][Hh][Aa][Nn][Kk][Ss][!！]?\\s*$" line) #false]
          [(regexp-match? #px"Copyright@" line) #false]
          [else #true])))

(define courseware-filter : (-> Input-Port Output-Port Void)
  (lambda [/dev/cwin /dev/cwout]
    (let filter ([last-state : Symbol '||])
      (define line (read-line /dev/cwin))

      (unless (eof-object? line)
        (cond [(regexp-match? #px"^\\s*$" line)
               (unless (eq? last-state 'blank)
                 (courseware-write-line /dev/cwout line))
               (filter 'blank)]
              [(regexp-match? #px"^/Volumes/VirtualServer" line)
               (courseware-write-line /dev/cwout line)
               (filter 'new-record)]
              [(eq? last-state 'new-record)
               (cond [(= (string-length line) 40) (courseware-write-line /dev/cwout line)]
                     [else (let ([sha1 (substring line 0 40)]
                                 [nxtl (substring line 40)])
                             (displayln sha1 /dev/cwout)
                             (courseware-write-line /dev/cwout nxtl))])
               (filter '||)]
              [(courseware-string-okay? line)
               (courseware-write-line /dev/cwout line)
               (filter '||)]
              [else (filter last-state)])))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define-values (options λargv) (parse-courseware-flags (current-command-line-arguments)))
  
  (when (courseware-flags-help? options)
    (display-courseware-flags #:exit 0))
  
  (parameterize ([current-custodian (make-custodian)])
    (dynamic-wind
     (λ [] (when (courseware-flags-dest options)
             (define o (courseware-flags-dest options))
             (make-parent-directory* o)
             (current-output-port (open-output-file #:exists 'truncate/replace
                                                    o))))
     
     (λ [] (let ([source (λargv)])
             (call-with-input-file (path->complete-path source)
               (λ [[/dev/cwin : Input-Port]]
                 (courseware-filter /dev/cwin
                                    (current-output-port))))))
    
     (λ [] (custodian-shutdown-all (current-custodian))))))
