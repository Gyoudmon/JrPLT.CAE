#lang info

(define collection 'use-pkg-name)
(define pkg-desc "Youth Language")

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)

(define sdl2-config
  '((config SDL2)
    (include [windows "village\\vcso\\include"])
    (libpath [windows "village\\vcso\\lib"])))

(define native-compiled-subpath '())

(define native-launcher-names
  `(["digitama/big_bang/BigBang.cpp" so ,sdl2-config]
    ["BigBang.cpp" console ,sdl2-config]
    ["FontBrowser.cpp" console ,sdl2-config]
    ["village/procedural/shape.cpp" console ,sdl2-config]
    ["village/procedural/paddleball.cpp" console ,sdl2-config]))
