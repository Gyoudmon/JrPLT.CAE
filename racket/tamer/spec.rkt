#lang typed/racket/base

(require digimon/spec)

(require "cc/chksum.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define ipv4-headers
  (list (bytes #x45 #x00 #x00 #x73 #x00 #x00 #x40 #x00 #x40 #x11
               #x00 #x00 #xc0 #xa8 #x00 #x01 #xc0 #xa8 #x00 #xc7)
        (bytes #x45 #x00 #x00 #x1c #x74 #x68 #x00 #x00 #x80 #x11
               #x59 #x8f #xc0 #xa8 #x64 #x01 #xab #x46 #x9c #xe9)))

(define message
  (bytes #x00 #x01 #xf2 #x03 #xf4 #xf5 #xf6 #xf7))

(define-feature prelude #:do
  (describe "Checksum" #:do
    (describe "CRC32" #:do
      (it "should return zero for empty message" #:do
        (expect-bytes= (str_chksum_crc32 #"") #"00000000"))
      (it "should return '0x414FA339' for 'The quick brown fox jumps over the lazy dog'" #:do
        (expect-bytes= (str_chksum_crc32 #"The quick brown fox jumps over the lazy dog") #"414FA339"))
      (it "should return '0x7E450C04' for '73871727080876A0'" #:do
        (expect-0x= (hex_chksum_crc32 #"73871727080876A0") #x7E450C04))
      (it "should return '0x2e8885d5' for 'NO4D061320000830BEB9BFE3C7C6CE68B16411FD09F96982'" #:do
        (expect-0x= (hex_chksum_crc32 #"NO4D061320000830BEB9BFE3C7C6CE68B16411FD09F96982") #x2e8885d5))
      (it "should return '0x2e8885d5' for the splitted long message" #:do
        (expect-0x= (acc_chksum_crc32 #"NO4D0613" #"20000830" #"BEB9BFE3C7C6CE68B16411FD09F96982") #x2e8885d5)))
    (describe "ipv4/tcp/udp" #:do
      (it "should be accumulative" #:do
        (let ([sum (hex_chksum_ipv4 message)])
          (for ([idx (in-range (bytes-length message))])
            (expect-= (acc_chksum_ipv4 message idx) sum
                      (format "broken@~a" idx)))))
      (it "should return zero for header's 1's complement code when verifying the checksum" #:do
        (for ([header (in-list ipv4-headers)])
          (expect-zero (chksum_ipv4_verify header))))
      (it "should return '0xFFFF' for header's tru-form sum when verifying the checksum" #:do
        (for ([header (in-list ipv4-headers)])
          (expect-= (chksum_ipv4_true_form header) #xFFFF))))))





(module+ main
  (void (spec-prove prelude)))