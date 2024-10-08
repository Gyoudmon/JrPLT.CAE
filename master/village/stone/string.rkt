#lang typed/racket

(require bitmap)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define escape-chars : (HashTable Char String)
  (make-hasheq '([#\nul . "\\0"]
                 [#\tab . "\\t"]
                 [#\newline . "\\n"])))

(define make-chars-array : (->* ((U (Listof Char) String)) (Positive-Byte #:size Real #:gapsize Nonnegative-Real #:fill Fill-Paint #:esc-fill Fill-Paint) Bitmap)
  (lambda [src0 [max-length 1] #:size [size 24] #:gapsize [gap -1.0] #:fill [fill-color #false] #:esc-fill [esc-fill #false]]
    (define src : (Listof Char) (if (list? src0) src0 (append (string->list src0) (list #\nul))))
    (define midx : Index (max max-length (length src)))
    (define font : Font (desc-font #:family 'monospace #:size size))
    (define gridsize : Flonum (* (real->double-flonum size) 1.618))
    (define grid : Bitmap (bitmap-square gridsize #:fill fill-color))
    (define terminate-grid : Bitmap (if (not esc-fill) grid (bitmap-square gridsize #:fill esc-fill)))

    (bitmap-hc-append* #:gapsize gap
                       (let char->bitmap : (Listof Bitmap) ([cs : (Listof Char) src]
                                                            [idx : Nonnegative-Fixnum 0]
                                                            [spmb : (Listof Bitmap) null])
                         (cond [(>= idx midx) (reverse spmb)]
                               [(pair? cs)
                                (let* ([c : Char (car cs)]
                                       [terminator? : Boolean (hash-has-key? escape-chars c)]
                                       [s : String (hash-ref escape-chars c (λ [] (string c)))])
                                  (char->bitmap (cdr cs) (+ idx 1)
                                                (cons (bitmap-cc-superimpose (if (and terminator? esc-fill) terminate-grid grid)
                                                                             (bitmap-text s font))
                                                      spmb)))]
                               [else (char->bitmap null (+ idx 1) (cons grid spmb))])))))

(define bitmap-text* : (->* (String Font) (Real) Bitmap)
  (lambda [text font [size -2.0]]
    (define content (bitmap-text #:ascent 'magenta #:descent 'blue #:capline 'orange #:meanline 'green #:baseline 'red
                                 text (desc-font (desc-font font #:size 'xx-large) #:size size)))
    (bitmap-frame content)))

(define chars-array (make-chars-array (list #\我 #\是 #\一 #\个 #\汉 #\字 #\字 #\符 #\数 #\组) 12))
(define string-array (make-chars-array "我是一个汉字字符串"  12 #:esc-fill 'lightskyblue))

(define font-example
  (bitmap-vr-append* #:gapsize 16.0
                     (for/list : (Listof Bitmap) ([family (in-list '(monospace sans-serif serif math))])
                       (define font (desc-font #:family family #:variant 'small-caps))
                       (bitmap-text* (format "~a[~a]: Sphinx 0123456789" (font-face->family (font-face font)) (font-face font)) font))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  chars-array
  string-array
  font-example)
