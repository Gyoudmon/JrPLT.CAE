#lang racket/base

(provide (all-defined-out))
(provide (all-from-out digimon/tamer))

(require digimon/tamer)
(require digimon/collection)

(require scribble/manual)

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
