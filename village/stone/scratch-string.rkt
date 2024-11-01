#lang typed/racket

(require bitmap)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define font : Font (desc-font #:family 'monospace #:size 24))
(define subfont : Font (desc-font #:family 'monospace #:size (* (font-size font) 0.32) #:weight 'bold))

(define make-chars-array : (->* (String) (Positive-Byte #:gapsize Nonnegative-Real #:index (Option Byte) #:index-color Color #:fill Fill-Paint) Bitmap)
  (lambda [src0 [max-length 1] #:gapsize [gap -1.0] #:fill [fill-color #false] #:index [index 1] #:index-color [idx-color 'royalblue]]
    (define src : (Listof Char) (string->list src0))
    (define midx : Index (max max-length (length src)))
    (define gridsize : Flonum (* (real->double-flonum (font-size font)) 1.618))
    (define subsize : Flonum (real->double-flonum (font-size subfont)))
    (define grid : Bitmap (bitmap-square gridsize #:fill fill-color))
    (define subgrid : Bitmap (bitmap-square subsize #:stroke 'transparent))

    (bitmap-hc-append* #:gapsize gap
                       (let char->bitmap : (Listof Bitmap) ([cs : (Listof Char) src]
                                                            [idx : Nonnegative-Fixnum 0]
                                                            [spmb : (Listof Bitmap) null])
                         (cond [(>= idx midx) (reverse spmb)]
                               [(pair? cs)
                                (char->bitmap (cdr cs) (+ idx 1)
                                              (let ([self (bitmap-cc-superimpose grid (bitmap-text (car cs) font))])
                                                (cons (cond [(not index) self]
                                                            [else (let ([idx.bmp (bitmap-text #:color idx-color (+ index idx) subfont)])
                                                                    (bitmap-rb-superimpose (bitmap-cc-superimpose subgrid idx.bmp)
                                                                                           self))])
                                                      spmb)))]
                               [else (char->bitmap null (+ idx 1) (cons grid spmb))])))))

(define bitmap-concatenate : (-> String String Bitmap)
  (lambda [lhs rhs]
    (bitmap-hc-append (make-chars-array lhs)
                      (bitmap-text " + " font)
                      (make-chars-array rhs)
                      (bitmap-text " = " font)
                      (make-chars-array (string-append lhs rhs)))))

(define bitmap-char-at : (-> String Positive-Index Bitmap)
  (lambda [txt idx]
    (define ghost (bitmap-square (font-size subfont) #:stroke 'transparent))
    
    (bitmap-hc-append (make-chars-array txt)
                      (bitmap-text (format "[~a] = " idx) font)
                      (make-chars-array (substring txt (- idx 1) idx)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define scratch-string (make-chars-array "我是一个字符串"))

(define cat (bitmap-concatenate "古典" "书屋"))
(define ref (bitmap-char-at "三年级2班" 4))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  scratch-string
  cat
  ref)
