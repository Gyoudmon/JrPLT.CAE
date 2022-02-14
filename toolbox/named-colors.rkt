#lang typed/racket/base

(require bitmap)

(require racket/format)
(require racket/list)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define color-name->row : (-> Symbol (Listof Bitmap))
  (let ([font (desc-font #:family 'monospace #:size 'x-large)])
    (lambda [name]
      (define c (rgb* name))
      (list (bitmap-square (font-size font) #:fill c)
            (bitmap-text (symbol->string name) font)
            (bitmap-text (~a '0x (string-upcase (~r (flcolor->hex c) #:base 16 #:min-width 6 #:pad-string "0"))) font)))))
  
(define color-tables : (-> (Listof Symbol) Positive-Byte Bitmap)
  (lambda [all-names col]
    (bitmap-table*
     (let partition : (Listof (Listof Bitmap)) ([swor : (Listof (Listof Bitmap)) null]
                                                [names : (Listof Symbol) (sort all-names symbol<?)])
       (cond [(> (length names) col) (partition (cons (apply append (map color-name->row (take names col))) swor) (drop names col))]
             [(pair? names) (partition (cons (apply append (map color-name->row names)) swor) null)]
             [else (reverse swor)]))
     'lc 'cc 16.0 4.0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define named-colors : Bitmap
  (color-tables '(black darkgrey lightgrey grey white
                        darkred darkgreen darkyellow darkblue brown darkmagenta
                        darkcyan red lightred green lightgreen yellow lightyellow
                        blue lightblue magenta lightmagenta cyan lightcyan)
                2))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  named-colors)
