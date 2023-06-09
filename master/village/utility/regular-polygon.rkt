#lang typed/racket

(require bitmap)

(define R : Flonum 32.0)

(define regular-polygon : (-> Index Flonum Bitmap)
  (lambda [n pi-m]
    (bitmap-cc-superimpose (bitmap-regular-polygon n R (* pi pi-m) #:inscribed? #true #:fill 'darkorange)
                           (bitmap-circle R #:fill 'ghostwhite))))

(define polygon-r3 (regular-polygon 3 -0.5))
(define polygon-r4 (regular-polygon 4 -0.25))
(define polygon-r6 (regular-polygon 6 -0.5))

(module+ main
  polygon-r3
  polygon-r4
  polygon-r6)