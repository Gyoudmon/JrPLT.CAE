#lang info

(define collection 'use-pkg-name)
(define pkg-desc "Youth Language")

(define version "1.0")
(define pkg-authors '(wargrey))
(define test-omit-paths 'all)

(define msvc-kits-rootdir "C:\\Program Files (x86)\\Windows Kits\\10")
(define msvc-kits-version "10.0.19041.0")

(define native-launcher-names '(["main.cpp"
                                 (lib SDL2 SDL2main)]))
