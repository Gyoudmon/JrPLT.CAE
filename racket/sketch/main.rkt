#!/usr/bin/env racket

;;; The 1st line above is called SheBang in Unix
;;; It tells the Unix system to employ `racket` to run this script
;;;   starting from the second line, even if it is a blank line

#lang racket
;;; The 7th line above tells `racket` this script is written in the default 'racket' language

(define (main argc argv)
  (displayln "Hello, World!")
  0)



(module+ main
  ;;; the submodule with the required name 'main' can be used to launch the application
  ;;; just similar to what C and C++ applications would do with the 'main' function

  (exit (main (vector-length (current-command-line-arguments))
              (current-command-line-arguments))))

