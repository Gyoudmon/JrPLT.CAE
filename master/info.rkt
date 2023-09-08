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
    (lib gydm.stem)
    (include [windows "C:\\opt\\GYDMstem\\include"]
             [macosx "/opt/GYDMstem/include"])
    (libpath [windows "C:\\opt\\GYDMstem\\lib"]
             [macosx "/opt/GYDMstem/lib"])))

(define native-compiled-subpath '())

(define native-launcher-names
  `(["GradeManagementSystem.cpp" console ,sdl2-config]
    ["YouthLanguageLobby.cpp" console ,sdl2-config]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define typesettings
  '(["literacy/Setup.scrbl" xelatex #px"Installation/.*png$"]
    ["literacy/Disciplines.scrbl" xelatex #px"Discipline/.*png$"]
    ["literacy/Vocabulary.scrbl" xelatex]
    ["literacy/Metrics.scrbl" xelatex #:always-make]))

#;(define literacy-samples '(["literacy/Setup/sync.scrbl"]))
