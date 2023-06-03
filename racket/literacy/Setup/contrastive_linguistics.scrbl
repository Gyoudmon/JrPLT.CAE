#lang scribble/manual

@(require "../literacy.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{对比语言学}

假设存在一个包罗万象的程序语言，
那我们学习任何一门具体的程序语言时会碰到的东西
也一定能在学习这个假想的语言时碰到。
@idea{如果一定要从语言角度来解释"如何学编程"，
那就是学习这个包括万象的假想语言，
并知道如何将它翻译成老板/客户希望你用的具体的语言。}

基于以上理念，本章主要通过对比 C++ 和 Python 来认识程序语言中的基本概念。

@include-section{contrastive/variable.scrbl}
@include-section{contrastive/type.system.scrbl}
@include-section{contrastive/enumeration.scrbl}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
