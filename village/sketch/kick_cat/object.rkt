#lang racket

; 场景：踢猫效应

; 自然语言描述：孩子很气愤，就用左脚踢了身边的猫
; 提取关键词：左脚 踢 猫 => 左 踢 猫
; 调整语序：踢 猫 左

; 提出异议，缺少主语。

; 1. 可以将主语信息以补语形式给出
(define (踢 猫 谁 哪只)
  ; 根据函数签名，还原自然语言描述
  (printf "用~a的~a脚踢了~a, ~a很气愤"
          谁 哪只 猫 猫))

; 2. 主语作为“对象”

; 角色：老板
;   功能： 踢人
;   属性： 名字
;         地位
;         性别

; 定义类和方法
(define bad-person%
  (class object% (super-new)
    ; 坏人需要有名字，方便通报批评
    (init-field name)

    (printf "~a已就绪~n" name)

    ; 坏人会踢人
    ; 方法令人不齿：欺软怕硬，欺负弱小
    ; 不过，坏人一旦踢人，我们就都知道坏人是谁了
    (define/public (踢 弱者 这只)
      (printf "~a用~a脚踢了~a, ~a很气愤~n"
              name 这只 弱者 弱者))))

; 角色就绪 实例化，找个人扮演角色，得到东西较“对象”，绑定给变量
(define 老板 (new bad-person% [name "老板"]))
(define 员工 (new bad-person% [name "员工"]))
(define 孩子 (new bad-person% [name "孩子"]))

; 完整还原情景
(send 老板 踢 (get-field name 员工) "左")
(send 员工 踢 (get-field name 孩子) "左")
(send 孩子 踢 "猫" "左")
