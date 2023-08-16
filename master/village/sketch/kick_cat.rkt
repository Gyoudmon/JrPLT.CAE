#lang racket

; 场景：踢猫效应

; 自然语言描述：孩子很气愤，就用左脚踢了身边的猫
; 提取关键词：左脚 踢 猫 => 左 踢 猫
; 调整语序：踢 猫 左

; 定义函数：
(define (踢 猫 左)
  ; 根据函数签名，还原自然语言描述
  ; kick the cat with left foot
  (printf "用~a脚踢了~a~n" 左 猫))

; 调用函数：
(踢 '加菲猫 '左)
(踢 '波斯猫 '右)

; 古灵精怪：
(踢 '狗 '中)

; 发现问题，引出“类型”概念，并适当调整参数名
; 介绍 Typed Racket

(module safe typed/racket
  ; 定义猫属超类
  (struct 猫 ([name : Symbol]))
  
  (define (踢 [猫 : 猫] [脚 : (U '左 '右)])
    (printf "用~a脚踢了~a~n" 脚 (猫-name 猫)))

  (踢 (猫 '缅因猫) '左)
  (踢 (猫 '暹罗猫) '右)
  #;(踢 '猫 '中))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 提出异议，缺少主语。

(module kick racket/base
  ; 1. 可以将主语信息以补语形式给出
  (define (踢 弱者 脚 谁)
    ; 根据函数签名，还原自然语言描述
    (printf "~a用~a脚踢了~a, ~a很气愤~n" 谁 脚 弱者 弱者)))

; 引出“对象”概念

; 定义类和方法
(define person%
  (class object% (super-new)
    (init-field [name 5])
    
    (define/public (踢 谁 脚)
      (printf "~a用~a脚踢了~a, ~a很气愤~n" name 脚 谁 谁))))

; 角色就绪
(define 老板 (new person% [name '老板]))
(define 老爸 (new person% [name '老爸]))
(define 孩子 (new person% [name '孩子]))

; 完整还原情景
(send 老板 踢 '老爸 '左)
(send 老爸 踢 '孩子 '左)
(send 孩子 踢 '猫 '左)
