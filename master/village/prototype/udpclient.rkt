#lang racket

(require racket/udp)

(define client (udp-open-socket))
(udp-send-to client "127.0.0.1" 2333 #"Hello, SDL2!")
(udp-send-to client "127.0.0.1" 2333 #"I am Racket!")
