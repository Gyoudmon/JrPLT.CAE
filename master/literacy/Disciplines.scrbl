#lang scribble/sigplan @nocopyright

@require{literacy.rkt}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title{青少计算机科学课程简介}

程序设计对于未成年人和成年人来说，都是足够新颖的技能，
零基础的这两个群体在初学阶段没有太大的认知差别。
核心问题在于，如何帮助学生(包括成年人和未成年人)建立新的思维模式。
这个过程的长短显然因人而异，因此，我的计算机科学课会尝试解决这个问题，
并找到一个平衡点，既要有铺垫时间来给初学者训练基础技能(如英文键盘打字)、
认识基本概念(如变量、语法和语义)、感受编程的乐趣；
又不能铺垫太长导致初学者耐心耗尽被动劝退。
而且，这个问题的解决只能从更高维度入手(比如课程的整体设计和教学过程的实施)，
而不是简单地带学生读懂每一行代码的语法细节。

本手册的主要读者是青少年计算机科学课系列课程的所有教师和所有市场人员。

@handbook-smart-table[]

@include-section{preface.scrbl}
@include-section{Disciplines/characteristics.scrbl}
@include-section{Disciplines/big-bang.scrbl}
@;include-section{Disciplines/faq.scrbl}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #true #:numbered? #false]
