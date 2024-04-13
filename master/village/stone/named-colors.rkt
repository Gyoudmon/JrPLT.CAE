#lang typed/racket/base

(require bitmap)

(require racket/format)
(require racket/list)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-type ColorName (U Symbol (Pairof Symbol Symbol)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define color-name->row : (-> ColorName (Listof Bitmap))
  (let ([font (desc-font #:family 'monospace #:size 'x-large)])
    (lambda [cn]
      (define-values (key name)
        (cond [(symbol? cn) (values cn cn)]
              [else (values (car cn) (cdr cn))]))
      
      (define c (rgb* key))
      (list (bitmap-square (font-size font) #:fill c)
            (bitmap-text (symbol->string name) font)
            (bitmap-text (~a '0x (string-upcase (~r (flcolor->hex c) #:base 16 #:min-width 6 #:pad-string "0"))) font)))))

(define color-key : (-> ColorName Symbol)
  (lambda [c]
    (cond [(symbol? c) c]
          [else (car c)])))
  
(define color-tables : (-> (Listof ColorName) Positive-Byte Bitmap)
  (lambda [all-names col]
    (bitmap-table*
     (let partition : (Listof (Listof Bitmap)) ([swor : (Listof (Listof Bitmap)) null]
                                                [names : (Listof ColorName) ((inst sort ColorName Symbol) all-names symbol<? #:key color-key)])
       (cond [(> (length names) col) (partition (cons (apply append (map color-name->row (take names col))) swor) (drop names col))]
             [(pair? names) (partition (cons (apply append (map color-name->row names)) swor) null)]
             [else (reverse swor)]))
     'lc 'cc 16.0 4.0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define named-colors : Bitmap
  (color-tables '(black darkgrey lightgrey grey white
                        darkred darkgreen darkblue darkmagenta
                        darkcyan red green lightgreen yellow lightyellow
                        blue lightblue magenta cyan lightcyan ghostwhite
                        lime orange chocolate gold purple violet darkviolet)
                2))

(define all-named-colors : Bitmap
  (color-tables (list-color-names) 3))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(module+ main
  named-colors
  all-named-colors

  (for ([c (in-list (list-color-names))])
    (printf "#define ~a 0x~aU~n"
            (string-upcase (symbol->string c))
            (number->string (flcolor->hex c) 16)))

  (newline)
  (for ([c (in-list (list-color-names))])
    (printf "~a = 0x~a~n"
            (string-upcase (symbol->string c))
            (number->string (flcolor->hex c) 16))))
