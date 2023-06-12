#lang typed/racket/base

(require digimon/spec)

(require "cc/chksum.rkt")
(require "cc/asnder.rkt")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define ipv4-headers
  (list (bytes #x45 #x00 #x00 #x73 #x00 #x00 #x40 #x00 #x40 #x11
               #x00 #x00 #xc0 #xa8 #x00 #x01 #xc0 #xa8 #x00 #xc7)
        (bytes #x45 #x00 #x00 #x1c #x74 #x68 #x00 #x00 #x80 #x11
               #x59 #x8f #xc0 #xa8 #x64 #x01 #xab #x46 #x9c #xe9)))

(define message
  (bytes #x00 #x01 #xf2 #x03 #xf4 #xf5 #xf6 #xf7))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
      (it "should return '0xFFFF' for header's true-form sum when verifying the checksum" #:do
        (for ([header (in-list ipv4-headers)])
          (expect-= (chksum_ipv4_true_form header) #xFFFF)))))

  (describe "ASN.1 DER" #:do
    (describe "Base" #:do
      (describe "Length" #:do
        (it-tame-length 127 #"\x7F")
        (it-tame-length 128 #"\x81\x80")
        (it-tame-length 201 #"\x81\xC9")
        (it-tame-length 435 #"\x82\x01\xB3")))
    (describe "Primitive Data Type" #:do
      (describe "Integer" #:do
        (context "Fixed Number" #:do
          (it-tame-fixnum 0 #"\x02\x01\x00")
          (it-tame-fixnum +1 #"\x02\x01\x01")
          (it-tame-fixnum -1 #"\x02\x01\xFF")
          (it-tame-fixnum +127 #"\x02\x01\x7F")
          (it-tame-fixnum -127 #"\x02\x01\x81")
          (it-tame-fixnum +128 #"\x02\x02\x00\x80")
          (it-tame-fixnum -128 #"\x02\x01\x80")
          (it-tame-fixnum +255 #"\x02\x02\x00\xFF")
          (it-tame-fixnum -255 #"\x02\x02\xFF\x01")
          (it-tame-fixnum +256 #"\x02\x02\x01\x00")
          (it-tame-fixnum -256 #"\x02\x02\xFF\x00")
          (describe "Enumeration" #:do
            (it-tame-enum 'Debug #"\x0A\x01\x00")
            (it-tame-enum 'Error #"\x0A\x01\x04")))
        (context "Natural" #:do
          (it-tame-natural #"807fbc" #"\x02\x04\x00\x80\x7f\xbc")
          (it-tame-natural ; (expt 17 80)
           #"7fbc8ce9af7a9eb54c817fc7c1c796d1b1c80bddbcbacb15942480f5aa4ee120d27f93ebcf43275d01"
           (bytes #x02 #x29 #x7f #xbc #x8c #xe9 #xaf #x7a #x9e #xb5
                  #x4c #x81 #x7f #xc7 #xc1 #xc7 #x96 #xd1 #xb1 #xc8
                  #x0b #xdd #xbc #xba #xcb #x15 #x94 #x24 #x80 #xf5
                  #xaa #x4e #xe1 #x20 #xd2 #x7f #x93 #xeb #xcf #x43
                  #x27 #x5d #x01))))

      (describe "Floating Number" #:do
        (it-tame-flonum +0.0 #"\x09\x00")
        (it-tame-flonum +inf.0 #"\x09\x01\x40")
        (it-tame-flonum -inf.0 #"\x09\x01\x41")
        (it-tame-flonum +nan.0 #"\x09\x01\x42")
        (it-tame-flonum -0.0 #"\x09\x01\x43")
        (it-tame-flonum 0.1 #"\x09\x09\x80\xC9\x0C\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum 0.2 #"\x09\x09\x80\xCA\x0C\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum 0.3 #"\x09\x09\x80\xCA\x13\x33\x33\x33\x33\x33\x33")
        (it-tame-flonum 0.4 #"\x09\x09\x80\xCB\x0C\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum 0.5 #"\x09\x03\x80\xFF\x01")
        (it-tame-flonum 0.6 #"\x09\x09\x80\xCB\x13\x33\x33\x33\x33\x33\x33")
        (it-tame-flonum 0.7 #"\x09\x09\x80\xCC\x0B\x33\x33\x33\x33\x33\x33")
        (it-tame-flonum 0.8 #"\x09\x09\x80\xCC\x0C\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum 0.9 #"\x09\x09\x80\xCB\x1C\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum 1.0 #"\x09\x03\x80\x00\x01")
        (it-tame-flonum 1.1 #"\x09\x09\x80\xCD\x08\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum 1.2 #"\x09\x09\x80\xCC\x13\x33\x33\x33\x33\x33\x33")
        (it-tame-flonum 1.3 #"\x09\x09\x80\xCC\x14\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum 1.4 #"\x09\x09\x80\xCD\x0B\x33\x33\x33\x33\x33\x33")
        (it-tame-flonum 1.5 #"\x09\x03\x80\xFF\x03")
        (it-tame-flonum 1.6 #"\x09\x09\x80\xCD\x0C\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum 2.718281828459045 #"\x09\x09\x80\xCD\x15\xBF\x0A\x8B\x14\x57\x69")
        (it-tame-flonum 3.141592653589793 #"\x09\x09\x80\xD0\x03\x24\x3F\x6A\x88\x85\xA3")
        (it-tame-flonum 0.5772156649015329 #"\x09\x09\x80\xCB\x12\x78\x8C\xFC\x6F\xB6\x19")
        (it-tame-flonum 1.618033988749895 #"\x09\x09\x80\xCF\x03\x3C\x6E\xF3\x72\xFE\x95")
        (it-tame-flonum 0.915965594177219 #"\x09\x09\x80\xCB\x1D\x4F\x97\x13\xE8\x13\x5D")
        (it-tame-flonum -0.0015625 #"\x09\x09\xC0\xC3\x0C\xCC\xCC\xCC\xCC\xCC\xCD")
        (it-tame-flonum -15.625 #"\x09\x03\xC0\xFD\x7D")
        (it-tame-flonum 180.0 #"\x09\x04\x80\x00\x00\xB4"))

      (describe "String" #:do
        (context "IA5 String" #:do
          (it-tame-ia5 '|6.0.5361.2| #"\x16\x0A\x36\x2E\x30\x2E\x35\x33\x36\x31\x2E\x32"))
        (context "UTF8 String" #:do
          (it-tame-utf8 "λsh\x0\nssh" #"\x0C\x09\xCE\xBB\x73\x68\x00\x0A\x73\x73\x68")))

      (describe "Miscellaneous" #:do
        (it-tame-null (void) #"\x05\x00")
        (it-tame-boolean #true #"\x01\x01\xFF")
        (it-tame-boolean #false #"\x01\x01\x00")))

    (describe "Sequence" #:do
      (it-tame-sequence 'Debug "测试" 1585280242148 'tamer
                        #"\x30\x1a\x0a\x01\x00\x0c\x06\xe6\xb5\x8b\xe8\xaf\x95\x02\x06\x01\x71\x1a\x10\xd1\xe4\x16\x05\x74\x61\x6d\x65\x72"))))





(module+ main
  (void (spec-prove prelude)))
