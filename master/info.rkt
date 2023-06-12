#lang info

(define collection "Youth Language")
(define pkg-desc "青少计算机科学")

(define deps '("digimon" "pict"))
(define build-deps '("digimon" "scribble-lib" "racket-doc"))

(define version "1.0")
(define pkg-authors '("WarGrey Gyoudmon Ju"))
(define test-omit-paths 'all)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define sdl2-config
  '((config SDL2)
    (include [windows "C:\\opt\\vcso\\include"])
    (libpath [windows "C:\\opt\\vcso\\lib"])))

(define native-compiled-subpath '())

(define native-launcher-names
  `(["YouthLanguageLobby.cpp" console ,sdl2-config]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define typesettings
  '(["literacy/ContrastiveLinguistics.scrbl" xelatex]
    ["literacy/Setup.scrbl" xelatex #px"Installation/.*png$"]
    ["literacy/Disciplines.scrbl" xelatex #px"Discipline/.*png$"]
    ["literacy/Metrics.scrbl" xelatex #:always-make]))

#;(define literacy-samples '(["literacy/Setup/sync.scrbl"]))