#lang info

(define collection 'use-pkg-name)
(define pkg-desc "Youth Language")

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)

(define native-launcher-names '(["BigBang.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "C:\\opt\\SDL2-2.0.18-vc\\include"])
                                  (libpath [windows "C:\\opt\\SDL2-2.0.18-vc\\lib\\x64"]))]
                                ["GamePlayer.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "C:\\opt\\SDL2-2.0.18-vc\\include"])
                                  (libpath [windows "C:\\opt\\SDL2-2.0.18-vc\\lib\\x64"]))]
                                ["DrawingBoard.cpp" console ;desktop
                                 ((config SDL2)
                                  (include [windows "C:\\opt\\SDL2-2.0.18-vc\\include"])
                                  (libpath [windows "C:\\opt\\SDL2-2.0.18-vc\\lib\\x64"]))]))

