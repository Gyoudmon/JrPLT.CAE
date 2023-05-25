#lang info

(define collection 'use-pkg-name)
(define pkg-desc "Youth Language")

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)

(define native-launcher-names
  '(["digitama/big_bang/BigBang.cpp"
     so
     ((config SDL2)
      (include [windows "village\\vcso\\include"])
      (libpath [windows "village\\vcso\\lib"]))]
    ["BigBang.cpp"
     console
     ((config SDL2)
      (include [windows "village\\vcso\\include"])
      (libpath [windows "village\\vcso\\lib"]))]
    ["FontBrowser.cpp"
     console
     ((config SDL2)
      (include [windows "village\\vcso\\include"])
      (libpath [windows "village\\vcso\\lib"]))]
    ["village/procedural/shape.cpp"
     console
     ((config SDL2)
      (include [windows "village\\vcso\\include"])
      (libpath [windows "village\\vcso\\lib"]))]
    ["village/procedural/paddleball.cpp"
     console
     ((config SDL2)
      (include [windows "village\\vcso\\include"])
      (libpath [windows "village\\vcso\\lib"]))]))
