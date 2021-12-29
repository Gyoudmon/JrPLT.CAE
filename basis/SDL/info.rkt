#lang info

(define collection 'use-pkg-name)
(define pkg-desc "Youth Language")

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)

(define msvc-kits-rootdir "C:\\Program Files (x86)\\Windows Kits\\10")
(define msvc-kits-version "10.0.19041.0")

(define native-launcher-names '(["main.cpp"
                                 ((config SDL2)
                                  (include (windows "C:\\opt\\SDL2-2.0.18-vc\\include"))
                                  (libpath (windows "C:\\opt\\SDL2-2.0.18-vc\\lib\\x64")))]))
