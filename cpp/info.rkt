#lang info

(define collection 'use-pkg-name)
(define pkg-desc "Youth Language")

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)

(define native-launcher-names '(["BigBang.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "village\\vcso\\include"])
                                  (libpath [windows "village\\vcso\\lib"]))]
                                ["Gallery.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "village\\vcso\\include"])
                                  (libpath [windows "village\\vcso\\lib"]))]
                                ["FontBrowser.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "village\\vcso\\include"])
                                  (libpath [windows "village\\vcso\\lib"]))]
                                ["village/pop/shape.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "village\\vcso\\include"])
                                  (libpath [windows "village\\vcso\\lib"]))]
                                ["village/pop/paddleball.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "village\\vcso\\include"])
                                  (libpath [windows "village\\vcso\\lib"]))]))