#lang racket

; 场景：踢猫效应

; 自然语言描述：孩子很气愤，就用左脚踢了身边的猫
; 提取关键词：左脚 踢 猫 => 左 踢 猫
; 调整语序：踢 猫 左

; 定义函数：
(define (kick cat left)
  (printf "kick the ~a with ~a foot~n" cat left))

; 调用函数：
(kick "Garfield" 'left)
(kick "Persian" 'right)

; 古灵精怪：
(kick 3.0 12)

; 发现问题，引出“类型”概念，并适当调整参数名
; 介绍 Typed Racket
