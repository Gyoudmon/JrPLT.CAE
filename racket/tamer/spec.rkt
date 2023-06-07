#lang typed/racket/base

(require digimon/spec)

(require "cc/chksum.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-feature prelude #:do
  (describe "checksum" #:do
            (describe "shared object for FFI" #:do
                      (context "seed" #:do))))
  




(module+ main
  (void (spec-prove prelude)))
