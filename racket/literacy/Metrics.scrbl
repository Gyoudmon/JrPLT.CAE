#lang scribble/sigplan @nocopyright

@(require "literacy.rkt")
@(require "graphviz.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title{计算机科学课程源码库量化指标}

@abstract{对软件项目的量化历来是软件工程的重点和难点，迄今为止行业已经总结出了数以百计的度量方法，有些极其不实用，
 有些极其晦涩难懂以至于只有极少数专家有望能理解。本文尝试确立一些简单易懂的度量参数，用以将源码库的工作量折算成课时费。
 将来等此课程成熟、源码库可以作为独立软件产品（比如 C++ 版的 Scratch）发布时，再重新按照软件项目的标准量化。}

@handbook-statistics[#:ignore '(#px"vcso") #:gitstat-width 450 #:gitstat-radius 80
                     #:altcolors '(["Racket" . "Green"]
                                   ["Python" . "Khaki"])]

@handbook-smart-table[]

@include-section{preface.scrbl}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-appendix[#:index-section? #true #:numbered? #false
 (book-bib-entry #:date "2015" #:edition "Eighth Edition"
                 "SE:APA" "Software Engineering: A Practition's Approach" "Roger S. Pressman, Bruce R. Maxim" "China Mechine Press")]
