#lang racket/gui

(provide (all-defined-out))

(require (submod bitmap/digitama/unsafe/platform unsafe))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define game%
  (class canvas%
    (init parent)
    (super-new [parent parent])

    (displayln (make-canvas-surface this))))

(define universe%
  (class frame%
    (init title width height)
    (super-new [parent #false]
               [label title]
               [width width]
               [height height])

    (define game (new game% [parent this]))
    
    (send* this
      (show #true)
      (center 'both))

    (define/public (construct argv)
      (void))

    (define/public (big-bang)
      (void))))
