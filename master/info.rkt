#lang info

(define collection "Linguisteen")
(define pkg-desc "青少计算机科学")

(define deps '("digimon" "pict"))
(define build-deps '("digimon" "scribble-lib" "racket-doc"))

(define version "1.0")
(define pkg-authors '("WarGrey Gyoudmon Ju"))
(define test-omit-paths 'all)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define sdl2-config
  '((config SDL2)
    (lib gydm)
    (include "C:\\opt\\GYDMstem\\include")
    (libpath "C:\\opt\\GYDMstem\\lib")))

(define stem-config
  '((config SDL2)
    (lib gydm stemos)
    (include "C:\\opt\\GYDMstem\\include")
    (libpath "C:\\opt\\GYDMstem\\lib")))

(define native-compiled-subpath '())
(define native-compiled-bindir '("bin"))
(define native-compiled-incdir '())
(define native-compiled-libdir '())
(define native-compiled-release '())
(define native-compiled-debug '("debug"))

(define native-launcher-names
  `(["LotteryMachine.cpp" console ,stem-config]
    ["STEMClassroom.cpp" console ,sdl2-config]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define typesettings
  '(["literacy/Setup.scrbl" xelatex #px"Installation/.*png$"]
    ["literacy/Disciplines.scrbl" xelatex #px"Discipline/.*png$"]
    ["literacy/Vocabulary.scrbl" xelatex]
    ["literacy/Metrics.scrbl" xelatex #:explicitly-make]))

#;(define literacy-samples '(["literacy/Setup/sync.scrbl"]))
