#lang racket

(provide (all-defined-out))

(require "game.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define primitive-shape%
  (class universe%
    (init width height)
    (super-new [title "Primitive Shape"] [width width] [height height])))
