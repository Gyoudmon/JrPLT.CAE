#lang typed/racket

; 场景：踢猫效应

; 自然语言描述：孩子很气愤，就用左脚踢了身边的猫
; 提取关键词：左脚 踢 猫 => 左 踢 猫
; 调整语序：踢 猫 左

; 定义函数：
(define (kick [animal : String] [which : (U 'left 'right)])
  (printf "kick the ~a with ~a foot~n" animal which))

(kick "wild cat" 'left)
(kick "leopard cat" 'right)
#;(kick 3.0 'middle)
