#lang typed/racket/base

(require digimon/cmdopt)
(require digimon/format)

(require racket/list)
(require racket/string)
(require racket/symbol)
(require racket/match)

(require "schema.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-cmdlet-option category-flags #: Category-Flags
  #:program 'category
  #:args [source]
          
  #:banner "categorize courses"
  #:once-each
  [])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-type STEM-Categories (Rec x (Listof (Pairof Symbol x))))

(define stem-categories : STEM-Categories
  (list (list 'Life
              (list 'cell)
              (list 'microbiology)
              (list 'physiology)
              (list 'plant)
              (list 'animal)
              (list 'genetics)
              (list 'ecology)
              (list 'nutriology)
              (list 'neuro))
        (list 'Matter
              (list 'matter
                    (list 'physics)
                    (list 'chemistry
                          (list 'inorganic)
                          (list 'organic)))
              (list 'mechanics)
              (list 'thermology)
              (list 'optics)
              (list 'acoustics)
              (list 'electromagnetism)
              (list 'energy))
        (list 'Space
              (list 'Universe
                    (list 'astrogeodynamics)
                    (list 'SolarSystem))
              (list 'Earth
                    (list 'geography)
                    (list 'geology)
                    (list 'atmosphere)))
        (list 'T&E
              (list 'machine)
              (list 'electronics)
              (list 'bionics)
              (list 'civil)
              (list 'astro)
              (list 'material)
              (list 'environment)
              (list 'energy-source)
              (list 'energy-saving))
        (list 'Programming
              (list 'scratch)
              (list 'python)
              (list 'arduino)
              (list 'STEMA)
              (list 'Robot))
        (list 'Lego)
        (list '3DPrint)
        (list 'Laboratory
              (list 'utility)
              (list 'instrument)
              (list 'kitchen))
        (list 'Misc)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define number-join : (-> (Listof Natural) Natural)
  (lambda [levels]
    (for/fold ([idx : Natural 0])
              ([lvl (in-list levels)])
      (+ (* idx 10) lvl))))

(define number-split : (-> Natural (Listof Natural))
  (lambda [nat]
    (let split ([n : Natural nat]
                [stigid : (Listof Byte) null])
      (cond [(<= n 0) stigid]
            [else (let-values ([(n-- r) (quotient/remainder n 10)])
                    (split n-- (cons r stigid)))]))))

(define display-categorization : (-> Void)
  (lambda []
    (let display ([levels : (Pairof Natural (Listof Natural)) (list 1)]
                  [disciplines : STEM-Categories stem-categories])
      (when (pair? disciplines)
        (define self (car disciplines))
        (define indent : String (~space (* (assert (sub1 (length levels)) index?) 2)))
        (define levels++ : (Pairof Natural (Listof Natural)) (cons (add1 (car levels)) (cdr levels)))
        
        (cond [(pair? self)
               (printf "~a~a.~a~n" indent (number->string (number-join (reverse levels))) (car self))
               (display (cons 1 levels) (cdr self))
               (display levels++ (cdr disciplines))]
              [(symbol? self)
               (printf "~a~a.~a~n" indent (number->string (number-join (reverse levels))) self)
               (display levels++ (cdr disciplines))])))))

(define number->categories : (-> Natural (Option (Pairof Symbol (Listof Symbol))))
  (lambda [n]
    (let map ([categories : STEM-Categories stem-categories]
              [digits : (Listof Natural) (number-split n)]
              [seman : (Listof Symbol) null])
      (if (pair? digits)
          (let ([idx (sub1 (car digits))])
            (and (and (>= idx 0)
                      (< idx (length categories)))
                 (let ([self (list-ref categories idx)])
                   (if (symbol? self)
                       (map null (cdr digits)
                            (cons self seman))
                       (map (cdr self) (cdr digits)
                            (cons (car self) seman))))))
          (let ([names (reverse seman)])
            (and (pair? names) names))))))

(define read-category-ids : (-> Input-Port (Listof Natural))
  (lambda [/dev/stdin]
    (printf "> ")
    (define line (read-line /dev/stdin))
    (if (string? line)
        (for/list ([token (in-list (string-split line))]
                   #:when (regexp-match? #px"\\d+" token))
          (assert (string->number token) exact-nonnegative-integer?))
        (list 0))))

(define read-categories : (-> Input-Port (Listof (Pairof Symbol (Listof Symbol))))
  (lambda [/dev/stdin]
    (display-categorization)

    (let try-read ([error-message : (Option String) #false])
      (when error-message (displayln error-message))
      (define ids (read-category-ids (current-input-port)))

      (cond [(and (pair? ids) (null? (cdr ids)) (= (car ids) 0)) null]
            [else (let ([all-cs (filter-map number->categories ids)])
                    (if (pair? all-cs)
                        all-cs
                        (try-read "invalid, retry.")))]))))

(define categories-join : (-> (Listof (Pairof Symbol (Listof Symbol))) String)
  (lambda [cs]
    (string-join
     (for/list : (Listof String) ([c (in-list cs)])
       (string-join (map symbol->immutable-string c) ","))
     ";")))

(define course-update : (-> Connection CourseWare Input-Port Void)
  (lambda [schema self /dev/stdin]
    (define cs (read-categories /dev/stdin))

    (when (pair? cs)
      (define categories : String (categories-join cs))

      (printf "updating with categories: ~a~n" categories)
      (courseware:update schema
                         (remake-courseware #:categories categories
                                            self)))))

(define course-categorize : (-> Connection Input-Port Void)
  (lambda [schema /dev/stdin]
    (define total (?courseware:count schema))
    
    (for ([self (in-courseware schema)]
          [idx (in-naturals 1)]
          #:when (courseware? self))
      (unless (courseware-categories self)
        (printf "******************************************************~n")
        (when (courseware-content self)
          (displayln (courseware-content self)))
        (printf "------------------------------------------------------~n")
        (printf "[~a(~a/~a)] ~a~n~a~n" (~% (/ idx total)) idx total (courseware-uuid self) (courseware-path self))
        (when (courseware-duplicates self)
          (displayln (courseware-duplicates self)))
        (printf "======================================================~n")
        
        (course-update schema self /dev/stdin)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define-values (options λargv) (parse-category-flags (current-command-line-arguments)))
  
  (when (category-flags-help? options)
    (display-category-flags #:exit 0))
  
  (define schema : Connection (sqlite3-connect #:database (λargv)))

  (with-handlers ([exn:break? void])
    (course-categorize schema (current-input-port)))
  
  (disconnect schema))
