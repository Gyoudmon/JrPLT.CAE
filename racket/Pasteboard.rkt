#!/usr/bin/env racket
#lang racket

(require "shape.rkt")  ; importing the current game

(module+ main
  ; The universe is born from the Chaos
  (define universe (make-object primitive-shape% 1200 800))
  
  ; Construct the world of game
  (send universe construct (current-command-line-arguments))
  
  ; The big bang!
  ; The main game loop starts here, and lasts until being stopped by player
  (send universe big-bang)
  
  ; Racket exits happily
  ; The universe is destroyed in pass
  ; now the void is in charge
  (exit 0))
