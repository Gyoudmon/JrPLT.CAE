#lang typed/racket/base

(provide (all-defined-out))

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-asnder (digimon-ffi-lib "asnder"))

  (define-asnder asn_tame_length
    (_fun (size expt) ::
          [size : _size]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o (bytes-length expt))]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _size]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_fixnum
    (_fun (fixnum expt) ::
          [fixnum : _int64]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o (bytes-length expt))]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _int64]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_natural
    (_fun (nat expt) ::
          [nat : _bytes/nul-terminated]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o (bytes-length expt))]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _bytes/nul-terminated]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_flonum
    (_fun (real expt) ::
          [real : _double]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o (bytes-length expt))]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _double]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_boolean
    (_fun (b expt) ::
          [b : _bool]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o (bytes-length expt))]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _bool]
          -> (values basn datum span offset)))

  (define-asnder asn_tame_null
    (_fun (any expt) ::
          [any : _?]
          [span : (_ptr o _size)]
          [offset : (_ptr o _size)]
          [basn : (_bytes o (bytes-length expt))]
          [bsize : _size = (bytes-length expt)]
          -> [datum : _void]
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
 [asn_tame_null (-> Void Bytes (Values Bytes Void Index Index))])

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
  #:it ["~a: λ ~a -> '~a'"
        type datum
        (bytes->hexstring #:separator "\\x" #:before-first "\\x" #:upcase? #true
                          expected-basn)]
  #:do (let-values ([(basn odatum span offset) (asn_tame datum expected-basn)]
                    [(expected-length) (bytes-length expected-basn)])
         (expect-bytes= basn expected-basn (format "asn_~a_to_octets" type))
         (expect-eqv odatum datum (format "asn_octets_to_~a (datum)" type))
         (expect-= offset expected-length (format "asn_octets_to_~a (offset)" type))
         (expect-= span expected-length (format "asn_~a_span" type))))

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
