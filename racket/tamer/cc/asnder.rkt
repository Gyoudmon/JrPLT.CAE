#lang typed/racket/base

(provide (all-defined-out))

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-asnder (digimon-ffi-lib "asnder"))

  (define _log
    (_enum '(Debug Info Notice Warning Error Critical Alarm Panic _)
           _uint16))

  (define-asnder asn_tame_length
    (_fun (size expt) ::
          [size : _size]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _size]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_fixnum
    (_fun (fixnum expt) ::
          [fixnum : _int64]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _int64]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_natural
    (_fun (nat expt) ::
          [nat : _bytes/nul-terminated]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          [datum : (_bytes o rsize)]
          [rsize : _size = (bytes-length nat)]
          [ten : _byte = 97]
          -> _void
          -> (values basn datum span offset)))

  (define-asnder asn_tame_flonum
    (_fun (real expt) ::
          [real : _double]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _double]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_boolean
    (_fun (b expt) ::
          [b : _bool]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _bool]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_null
    (_fun (any expt) ::
          [any : _?]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _void]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_ia5_string
    (_fun (ia5 expt) ::
          [ia5 : _symbol]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          [datum : (_bytes o rsize)]
          [rsize : _size = (string-length (symbol->string ia5))]
          -> _void
          -> (values basn (string->symbol (bytes->string/utf-8 datum)) span offset)))

  (define-asnder asn_tame_utf8_string
    (_fun (utf8 expt) ::
          [utf8 : _string/utf-8]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          [datum : (_bytes o rsize)]
          [rsize : _size = (string-utf-8-length utf8)]
          -> _void
          -> (values basn (bytes->string/utf-8 datum) span offset)))

  (define-asnder asn_tame_enum
    (_fun (log expt) ::
          [log : _log]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _log]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_sequence
    (_fun (level desc timestamp topic expt) ::
          [level : _log]
          [desc : _string/utf-8]
          [timestamp : _int64]
          [topic : _symbol]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o bsize)]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _bool]
          -> (values basn datum span offset))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(require digimon/spec)
(require digimon/format)

(require (for-syntax racket/base))

(require/typed/provide
 (submod "." unsafe)
 [asn_tame_length (-> Index Bytes (Values Bytes Index Index Index))]
 [asn_tame_fixnum (-> Integer Bytes (Values Bytes Integer Index Index))]
 [asn_tame_natural (-> Bytes Bytes (Values Bytes Bytes Index Index))]
 [asn_tame_flonum (-> Flonum Bytes (Values Bytes Flonum Index Index))]
 [asn_tame_boolean (-> Boolean Bytes (Values Bytes Boolean Index Index))]
 [asn_tame_null (-> Void Bytes (Values Bytes Void Index Index))]
 [asn_tame_ia5_string (-> Symbol Bytes (Values Bytes Symbol Index Index))]
 [asn_tame_utf8_string (-> String Bytes (Values Bytes String Index Index))]
 [asn_tame_enum (-> Symbol Bytes (Values Bytes Symbol Index Index))]
 [asn_tame_sequence (-> Symbol String Integer Symbol Bytes (Values Bytes Boolean Index Index))])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-behavior (it-tame-natural datum expected-basn)
  #:it ["natural: λ 0x~a -> '~a'"
        datum
        (bytes->hexstring #:separator "\\x" #:before-first "\\x" #:upcase? #true
                          expected-basn)]
  #:do (let-values ([(basn odatum span offset) (asn_tame_natural datum expected-basn)]
                    [(expected-length) (bytes-length expected-basn)])
         (expect-bytes= basn expected-basn "asn_natural_to_octets")
         (expect-bytes= odatum datum "asn_octets_to_natural (datum)")
         (expect-= offset expected-length "asn_octets_to_natural (offset)")
         (expect-= span expected-length "asn_natural_span")))

(define-behavior (it-tame-primitive datum expected-basn asn_tame type)
  #:it ["~a: λ ~s -> '~a'"
        type datum
        (bytes->hexstring #:separator "\\x" #:before-first "\\x" #:upcase? #true
                          expected-basn)]
  #:do (let-values ([(basn odatum span offset) (asn_tame datum expected-basn)]
                    [(expected-length) (bytes-length expected-basn)])
         (expect-bytes= basn expected-basn (format "asn_~a_to_octets" type))
         (if (string? datum)
             (expect-string= odatum datum (format "asn_octets_to_~a (datum)" type))
             (expect-eqv odatum datum (format "asn_octets_to_~a (datum)" type)))
         (expect-= offset expected-length (format "asn_octets_to_~a (offset)" type))
         (expect-= span expected-length (format "asn_~a_span" type))))

(define-behavior (it-tame-sequence level message timestamp topic expected-basn)
  #:it ["sequence: λ { ~s, ~s, ~s, ~s } -> '~a'"
        level message timestamp topic
        (bytes->hexstring #:separator "\\x" #:before-first "\\x" #:upcase? #true
                          expected-basn)]
  #:do (let-values ([(basn okay span offset) (asn_tame_sequence level message timestamp topic expected-basn)]
                    [(expected-length) (bytes-length expected-basn)])
         (expect-bytes= basn expected-basn "asn_sequence_to_octets")
         (expect-true okay "asn_octets_to_sequence (datum)")
         (expect-= offset expected-length "asn_octets_to_sequence (offset)")
         (expect-= span expected-length "asn_sequence_span")))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-syntax (it-tame-length stx)
  (syntax-case stx []
    [(_ datum expected-basn)
     (syntax/loc stx
       (it-tame-primitive datum expected-basn asn_tame_length "length"))]))

(define-syntax (it-tame-fixnum stx)
  (syntax-case stx []
    [(_ datum expected-basn)
     (syntax/loc stx
       (it-tame-primitive datum expected-basn asn_tame_fixnum "fixnum"))]))

(define-syntax (it-tame-flonum stx)
  (syntax-case stx []
    [(_ datum expected-basn)
     (syntax/loc stx
       (it-tame-primitive datum expected-basn asn_tame_flonum "flonum"))]))

(define-syntax (it-tame-boolean stx)
  (syntax-case stx []
    [(_ datum expected-basn)
     (syntax/loc stx
       (it-tame-primitive datum expected-basn asn_tame_boolean "boolean"))]))

(define-syntax (it-tame-null stx)
  (syntax-case stx []
    [(_ datum expected-basn)
     (syntax/loc stx
       (it-tame-primitive datum expected-basn asn_tame_null "null"))]))

(define-syntax (it-tame-ia5 stx)
  (syntax-case stx []
    [(_ datum expected-basn)
     (syntax/loc stx
       (it-tame-primitive datum expected-basn asn_tame_ia5_string "ia5"))]))

(define-syntax (it-tame-utf8 stx)
  (syntax-case stx []
    [(_ datum expected-basn)
     (syntax/loc stx
       (it-tame-primitive datum expected-basn asn_tame_utf8_string "utf8"))]))

(define-syntax (it-tame-enum stx)
  (syntax-case stx []
    [(_ datum expected-basn)
     (syntax/loc stx
       (it-tame-primitive datum expected-basn asn_tame_enum "enum"))]))
