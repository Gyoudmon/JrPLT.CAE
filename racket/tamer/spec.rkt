#lang typed/racket/base

(require digimon/spec)
(require digimon/checksum)

(require "cc/chksum.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-feature prelude #:do
  (describe "checksum" #:do
            (describe "CRC32" #:do
              (it "should return zero for empty message" #:do
                (expect-bytes= #"00000000" (str_chksum_crc32 #"")))
              (it "should return '0x414FA339' for 'The quick brown fox jumps over the lazy dog'" #:do
                (expect-bytes= #"414FA339" (str_chksum_crc32 #"The quick brown fox jumps over the lazy dog")))
              (it "should return '0x7E450C04' for '73871727080876A0'" #:do
                (expect-0x= #x7E450C04 (hex_chksum_crc32 #"73871727080876A0")))
              (it "should return '0x2e8885d5' for 'NO4D061320000830BEB9BFE3C7C6CE68B16411FD09F96982'" #:do
                (expect-0x= #x2e8885d5 (hex_chksum_crc32 #"NO4D061320000830BEB9BFE3C7C6CE68B16411FD09F96982")))
              (it "should return '0x2e8885d5' for the splitted long message" #:do
                (expect-0x= #x2e8885d5 (acc_chksum_crc32 #"NO4D0613" #"20000830" #"BEB9BFE3C7C6CE68B16411FD09F96982"))))))





(module+ main
  (void (spec-prove prelude)))