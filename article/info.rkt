#lang info

(define collection 'use-pkg-name)
(define pkg-desc "青少计算机科学")

(define deps '("digimon"))
(define build-deps '("digimon" "scribble-lib" "racket-doc"))

(define version "1.0")
(define pkg-authors '("WarGrey Gyoudmon Ju"))
(define test-omit-paths 'all)

(define typesettings '(["YouthLanguage.scrbl" xelatex]))

(define samples '(["literacy/sync.scrbl" 1]))

