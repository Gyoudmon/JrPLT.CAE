#lang typed/racket/base

(module unsafe racket/base
  (provide (all-defined-out))
  
  (require digimon/ffi)

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (define-ffi-definer define-chksum (digimon-ffi-lib "chksum"))

  (define-chksum hex_chksum_crc32 (_fun [message : _bytes/nul-terminated]
                                     [size : _size = (bytes-length message)]
                                     -> _uint32))
  
  (define-chksum str_chksum_crc32 (_fun [message : _bytes/nul-terminated]
                                     [size : _size = (bytes-length message)]
                                     -> _bytes/nul-terminated))

  (define-chksum acc_chksum_crc32 (_fun [pm1 : _bytes/nul-terminated]
                                     [psz1 : _size = (bytes-length pm1)]
                                     [pm2 : _bytes/nul-terminated]
                                     [psz2 : _size = (bytes-length pm2)]
                                     [pm3 : _bytes/nul-terminated]
                                     [psz3 : _size = (bytes-length pm3)]
                                     -> _uint32)))

(require/typed/provide
 (submod "." unsafe)
 [hex_chksum_crc32 (-> Bytes Natural)]
 [str_chksum_crc32 (-> Bytes Bytes)]
 [acc_chksum_crc32 (-> Bytes Bytes Bytes Natural)])
