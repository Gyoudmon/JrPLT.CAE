#lang racket/base

(provide (all-defined-out))
(provide (all-from-out digimon/tamer))

(require digimon/tamer)
(require digimon/collection)

(require scribble/core)
(require scribble/manual)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Just in case for README.md
(enter-digimon-zone!)

(tamer-indexed-block-hide-chapter-index #true)

(tamer-default-figure-label "图")
(tamer-default-figure-label-separator #false)
(tamer-default-figure-label-tail ". ")
(tamer-default-figure-label-style 'bold)
