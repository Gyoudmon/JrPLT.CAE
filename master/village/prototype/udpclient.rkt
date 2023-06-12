#lang racket

(require racket/udp)

(require digimon/checksum)
(require digimon/format)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define payload-idx 10)
(define checksum-idx 4)

(define slang-message-checksum
  (lambda [message]
    (define headsum (checksum-ipv4 message 0 checksum-idx))
    (define chksum (checksum-ipv4* message headsum (+ checksum-idx 2)))

    (if (zero? chksum) #xFFFF chksum)))

(define make-slang-message
  (lambda [payload id]
    (define message (make-bytes (+ (bytes-length payload) payload-idx) 0))

    (integer->integer-bytes #x237E 2 #false #true message 0)
    (bytes-set! message 2 1)
    (bytes-set! message 3 0)
    (integer->integer-bytes id 2 #false #true message 6)
    (integer->integer-bytes 0 2 #false #true message 8)
    (bytes-copy! message payload-idx payload 0)
    
    (integer->integer-bytes (slang-message-checksum message) 2 #false #true message checksum-idx)

    (printf "~a [CHECKSUM: ~a]~n" (~hexstring message) (~hexstring (checksum-ipv4 message)))
    message))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define client (udp-open-socket))

(for ([word (list* "Greetings, C++!" "I am Racket."
                   (vector->list (current-command-line-arguments)))]
      [id (in-naturals 1)])
  (udp-send-to client "127.0.0.1" 2333 (make-slang-message (string->bytes/utf-8 word) id)))
