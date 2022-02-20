#lang typed/racket/base

(require bitmap)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define tower-layer : (->* (String String Positive-Byte) (Real Real #:natural? Boolean) Bitmap)
 (lambda [caption keyword level [width 128.0] [offset 4.0] #:natural? [natural? #false]]
   (define label (bitmap-text caption))
   (define type (bitmap-text keyword #:color 'blue))
   (define gridsize (bitmap-height type))
   (define ch-width (/ (bitmap-width type) (string-length keyword)))
   (define label+type (bitmap-hc-append #:gapsize ch-width label type))
   (define box-width (+ width (* gridsize (* (- level 1) 2))))
   (define box-height gridsize)

   (bitmap-cc-superimpose (bitmap-rectangle box-width box-height) label+type)))

(define tower-layer/overlook : (->* (String String Positive-Byte) (Real Real #:natural? Boolean) Bitmap)
 (lambda [caption keyword level [width 100.0] [offset 4.0] #:natural? [natural? #false]]
   (define label (bitmap-text caption))
   (define type (bitmap-text keyword #:color 'blue))
   (define gridsize (bitmap-height type))
   (define ch-width (/ (bitmap-width type) (string-length keyword)))
   (define label+type (bitmap-hc-append #:gapsize ch-width label type))
   (define box-width (+ width (* gridsize (* (- level 1) 2))))
   (define box-height (* gridsize (* level 2)))

   (if (not natural?)
       (bitmap-pin-over (bitmap-rectangle box-width box-height) 4.0 4.0 label+type)
       (bitmap-pin-over (let ([half-height (* box-height 0.5)])
                          (bitmap-vl-append (bitmap-rectangle box-width half-height)
                                            (bitmap-blank box-width half-height)))
                        4.0 4.0 label+type))))

(define tower-wrap : (-> String Bitmap Bitmap)
  (lambda [title tower]
    (define label (bitmap-text title))
    (define gridsize (bitmap-height label))
    
    (bitmap-pin-over (bitmap-frame tower #:padding (list 16.0 32.0))
                     4.0 4.0 label)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define fixnum-tower : Bitmap
  (bitmap-vc-append
    (tower-layer "字节" "Char" 1)
    (tower-layer "短整型" "Short" 2)
    (tower-layer "整型" "int" 3)
    (tower-layer "长整型" "long long" 4)))

(define flonum-tower : Bitmap
  (bitmap-vc-append
    (tower-layer "单精度浮点数" "float" 1)
    (tower-layer "双精度浮点数" "double" 2)
    (tower-layer "扩展精度浮点数" "long double" 3)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  fixnum-tower
  flonum-tower)
