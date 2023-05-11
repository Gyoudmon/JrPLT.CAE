#lang racket/base

(provide (all-defined-out))
(provide (all-from-out digimon/tamer))

(require digimon/tamer)
(require digimon/collection)

(require scribble/manual)

(require (for-syntax racket/base))
(require (for-syntax racket/syntax))
(require (for-syntax syntax/parse))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Just in case for README.md
(enter-digimon-zone!)

;(tamer-indexed-block-hide-chapter-index #true)

(tamer-block-label-separator #false)
(tamer-block-label-tail ". ")
(tamer-block-label-style 'bold)

(tamer-default-figure-label "图")
(tamer-default-code-label "段")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-for-syntax (parse-optional-pair stx)
  (syntax-case stx []
    [(min max) (list #'min #'max)]
    [v #'v]))

(define-syntax (discipline-desc stx)
  (syntax-parse stx #:datum-literals []
    [(_ (~alt (~once (~seq #:langs [langs ...]))
              (~once (~seq #:grades [grades ...]))
              (~once (~seq #:period period))
              (~once (~seq #:note note)))
        ...)
     (with-syntax* ([(period:min period:max) (parse-optional-pair #'period)])
       (syntax/loc stx
         (nested #:style 'vertical-inset
                 (para (emph "语言：")
                       (add-between (list langs ...) " "))

                 (para (emph "学段：")
                       (add-between (list grades ...) " "))
                 
                 (para (emph "课时：")
                       (add-between (map (λ [v] (elem #:style "disTag" ($ (number->string v))))
                                         (filter exact-nonnegative-integer? (list period:min period:max)))
                                    " - "))

                 (para (emph "备注：") (emph note)))))]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define tag:C++ (elem #:style "langTag" "C++"))
(define tag:Python (elem #:style "langTag" "Python"))
(define tag:Scratch (elem #:style "langTag" "Scratch"))

(define tag:early (elem #:style "disTag" "小学低年级"))
(define tag:elementary (elem #:style "disTag" "小学高年级"))
(define tag:middle (elem #:style "disTag" "中学生"))
(define tag:teacher (elem #:style "disTag" "教师"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define stone-image
  (lambda [path #:scale [scale 1.0]]
    (image (digimon-path 'stone path) #:scale scale)))

(define tamer-c++
  (lambda [id caption subpath start [end #px"END"] [open? #true]]
    (tamer-code! #:open-range? open?
                 id caption (build-path ".." "cpp" "digitama" "IMS" subpath)
                 start end)))

(define tamer-c++-class
  (lambda [id caption subpath]
    (tamer-code-class id caption (build-path ".." "cpp" "digitama" "IMS" subpath))))

(define tamer-c++-function
  (lambda [id caption subpath #:ns [ns 'WarGrey::IMS] #:subpattern [subpattern #false]]
    (tamer-code-function #:ns ns #:subpattern subpattern
                         id caption (build-path ".." "cpp" "digitama" "IMS" subpath))))

(define cpp:function
  (lambda body
    (apply racketidfont body)))

(define cpp:variable
  (lambda body
    (apply racketvarfont body)))

(define cpp:type
  (lambda body
    (apply racketvalfont body)))
