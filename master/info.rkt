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
    (lib plteen)
    (include "C:\\opt\\JrPLT\\include")
    (libpath "C:\\opt\\JrPLT\\lib")))

(define stem-config
  '((config SDL2)
    (lib plteen stemos)
    (include "C:\\opt\\JrPLT\\include")
    (libpath "C:\\opt\\JrPLT\\lib")))

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
    ["literacy/Disciplines.scrbl" xelatex #px"Discipline/.*png$"]))
