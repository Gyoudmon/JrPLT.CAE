#lang info

(define collection "Youth Language")
(define pkg-desc "青少计算机科学")

(define deps '("digimon"))
(define build-deps '("digimon" "scribble-lib" "racket-doc"))

(define version "1.0")
(define pkg-authors '("WarGrey Gyoudmon Ju"))
(define test-omit-paths 'all)

(define typesettings '(["literacy/YouthLanguage.scrbl" xelatex]))

(define literacy-samples '(["literacy/sync.scrbl"]))
