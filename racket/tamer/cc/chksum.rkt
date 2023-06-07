#lang typed/racket/base

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-chksum (digimon-ffi-lib "chksum"))

  (define-chksum hex_chksum_crc32
    (_fun [message : _bytes/nul-terminated]
          [size : _size = (bytes-length message)]
          -> _uint32))
  
  (define-chksum str_chksum_crc32
    (_fun [message : _bytes/nul-terminated]
          [size : _size = (bytes-length message)]
          -> _bytes/nul-terminated))
  
  (define-chksum acc_chksum_crc32
    (_fun [pm1 : _bytes/nul-terminated]
          [psz1 : _size = (bytes-length pm1)]
          [pm2 : _bytes/nul-terminated]
          [psz2 : _size = (bytes-length pm2)]
          [pm3 : _bytes/nul-terminated]
          [psz3 : _size = (bytes-length pm3)]
          -> _uint32))

  (define-chksum hex_chksum_ipv4
    (_fun [message : _bytes/nul-terminated]
          [size : _size = (bytes-length message)]
          -> _uint16))

  (define-chksum acc_chksum_ipv4
    (_fun [message : _bytes/nul-terminated]
          [size : _size = (bytes-length message)]
          [idx : _size]
          -> _uint16))

  (define-chksum chksum_ipv4_verify
    (_fun [header : _bytes/nul-terminated]
          [size : _size = (bytes-length header)]
          -> _uint16))
  
  (define-chksum chksum_ipv4_true_form
    (_fun [header : _bytes/nul-terminated]
          [size : _size = (bytes-length header)]
          -> _uint16)))

(require/typed/provide
 (submod "." unsafe)
 [hex_chksum_crc32 (-> Bytes Nonnegative-Fixnum)]
 [str_chksum_crc32 (-> Bytes Bytes)]
 [acc_chksum_crc32 (-> Bytes Bytes Bytes Nonnegative-Fixnum)]

 [hex_chksum_ipv4 (-> Bytes Nonnegative-Fixnum)]
 [acc_chksum_ipv4 (-> Bytes Integer Nonnegative-Fixnum)]
 [chksum_ipv4_verify (-> Bytes Nonnegative-Fixnum)]
 [chksum_ipv4_true_form (-> Bytes Nonnegative-Fixnum)])
