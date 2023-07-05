#lang typed/racket/base

(require racket/string)
(require racket/file)

(require digimon/port)
(require digimon/cmdopt)

(require "schema.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-cmdlet-option export-flags #: Export-Flags
  #:program 'export
  #:args [source]
          
  #:banner "export courses from a sqlite database into a CSV file"
  #:once-each
  [[(#\a)      "append if output file exists"] 
   [(#\o dest) #:=> cmdopt-string->path dest #: Path
               "specific the path ~1 of the dateabase"]])

(define category-names : (HashTable Symbol String)
  (make-hasheq (list (cons 'Life "生命科学")
                     (cons 'cell "细胞生物学")
                     (cons 'microbiology "微生物学")
                     (cons 'physiology "生理学")
                     (cons 'plant "植物学")
                     (cons 'animal "动物学")
                     (cons 'genetics "遗传学")
                     (cons 'ecology "生态学")
                     (cons 'nutriology "营养学")
                     (cons 'neuro "神经科学")
                     (cons 'Matter "物质科学")
                     (cons 'matter "物质概述")
                     (cons 'physics "物理变化")
                     (cons 'chemistry "化学变化")
                     (cons 'inorganic "无机化学")
                     (cons 'organic "有机化学")
                     (cons 'mechanics "力学")
                     (cons 'thermology "热学")
                     (cons 'optics "光学")
                     (cons 'acoustics "声学")
                     (cons 'electromagnetism "电磁学")
                     (cons 'energy "能量")
                     (cons 'Space "空间科学")
                     (cons 'Universe "天文学")
                     (cons 'astrogeodynamics "天文地球动力学")
                     (cons 'SolarSystem "太阳系")
                     (cons 'Earth "地球科学")
                     (cons 'geography "地理学")
                     (cons 'geology "地质学")
                     (cons 'atmosphere "气象学")
                     (cons 'T&E "工程技术")
                     (cons 'machine "机械")
                     (cons 'electronics "电子技术")
                     (cons 'bionics "仿生学")
                     (cons 'civil "土木工程")
                     (cons 'astro "航天技术")
                     (cons 'material "材料技术")
                     (cons 'environment "环境工程")
                     (cons 'energy-source "能源")
                     (cons 'energy-saving "节能")
                     (cons 'Programming "编程")
                     (cons 'scratch "Scratch")
                     (cons 'python "Python")
                     (cons 'arduino "Arduino")
                     (cons 'STEMA "STEMA")
                     (cons 'Robot "机器人")
                     (cons 'Lego "科学乐高")
                     (cons '3DPrint "3D打印")
                     (cons 'Laboratory "实验室")
                     (cons 'utility "工具")
                     (cons 'instrument "器材")
                     (cons 'kitchen "厨具")
                     (cons 'Misc "其他"))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define course-row->csv-record : (-> (Pairof (Listof (Listof String)) (Vectorof String))
                                     Nonnegative-Fixnum Natural
                                     (Listof (Vectorof String)))
  (lambda [row clevel idx0]
    (define categories (car row))
    (define info (cdr row))
    (define info-idx (+ clevel 1))
    (define size (+ info-idx (vector-length info)))

    (for/list ([cs (in-list categories)]
               [idx (in-naturals idx0)])
      (define cv (make-vector size ""))
      
      (vector-set! cv 0 (if (< idx0 1) "序号" (number->string idx)))
      (vector-copy! cv info-idx info)
      (for ([c (in-list cs)]
            [i (in-naturals 1)])
        (vector-set! cv i c))
      
      cv)))

(define course-record-write : (-> (Vectorof String) Output-Port Void)
  (lambda [csv /dev/csvout]
    (displayln csv /dev/stdout)
    (for ([field (in-vector csv)])
      (fprintf /dev/csvout "~a," field))
    (newline /dev/csvout)
    (flush-output /dev/csvout)))

(define course-export : (-> Connection Output-Port Void)
  (lambda [schema /dev/csvout]
    (define-values (swor clevel)
      (for/fold ([swor : (Listof (Pairof (Listof (Listof String)) (Vectorof String))) null]
                 [level : Nonnegative-Fixnum 0])
                ([self (in-courseware schema)]
                 #:when (and (courseware? self)
                             (courseware-categories self)))
        (define category : String (courseware-categories self))
        (define categories : (Listof (Listof String))
          (for/list ([cs (in-list (string-split category ";"))])
            (for/list : (Listof String) ([f (in-list (string-split cs ","))])
              (hash-ref category-names (string->symbol f) (λ [] f)))))

        (define row : (Pairof (Listof (Listof String)) (Vectorof String))
          (cons categories
                (vector (courseware-path self) (courseware-sha1 self)
                        (let ([dups (courseware-duplicates self)])
                          (if dups (string-replace dups "\n" " | ") "")))))

        (values (cons row swor)
                (for/fold ([lvl : Nonnegative-Fixnum level])
                          ([cs (in-list categories)])
                  (max lvl (length cs))))))


    (define header (course-row->csv-record (cons (list null) (vector "路径" "指纹" "重复路径")) clevel 0))
    (when (pair? header)
      (course-record-write (car header) /dev/csvout))
    
    (for/fold ([idx : Natural 1])
              ([row (in-list (reverse swor))])
      (define records (course-row->csv-record row clevel idx))
      (for ([csv (in-list records)])
        (course-record-write csv /dev/csvout))
      (+ idx (length records)))

    (displayln (cons (length swor) clevel) /dev/stdout)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  (define-values (options λargv) (parse-export-flags (current-command-line-arguments)))
  
  (when (export-flags-help? options)
    (display-export-flags #:exit 0))
  
  (define schema : Connection (sqlite3-connect #:database (path->complete-path (λargv))))

  (parameterize ([current-custodian (make-custodian)])
    (dynamic-wind
     (λ [] (when (export-flags-dest options)
             (define o.csv (path->complete-path (export-flags-dest options)))
             (make-parent-directory* o.csv)
             (current-output-port (open-output-file #:exists (if (export-flags-a options)
                                                                 'append
                                                                 'truncate/replace)
                                                    o.csv))))

     (λ [] (course-export schema (current-output-port)))
     (λ [] (custodian-shutdown-all (current-custodian)))))
  
  (disconnect schema))
