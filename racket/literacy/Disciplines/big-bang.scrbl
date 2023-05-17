#lang scribble/manual

@(require "../literacy.rkt")

@(define figure-scale 0.20)

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{宇宙大爆炸}

@discipline-desc[
 #:langs [tag:C++ tag:Python]
 #:grades [tag:elementary tag:middle tag:teacher]
 #:period [24 32]
 #:note "对于有基础的学生，也需要从本课程中先修部分知识"]

@margin-note{取名为@tech{宇宙大爆炸}是顺应了宇宙发展“从无到有”的自然规律。}

@tamer-deftech{宇宙大爆炸}是青少计算机科学后续所有课程的基础。
学完本课程之后，学生将对程序语言、程序设计和计算思维有个初步的了解。

@tamer-figure!["d:bigbang" "宇宙大爆炸 主界面"]{
 @stone-image["Disciplines/big-bang.png" #:scale figure-scale]}

@tamer-figure-ref{d:bigbang}是@tech{宇宙大爆炸}的主界面。
其中，企鹅@racketerror{不代表腾讯}，而是 @emph{Linux} 操作系统的吉祥物；
鱼币是企鹅的食物，大部分鱼币代表学生作品，少部分是教师扩展程序(这些可随
源码库一起提供给学生课后深入学习用)。

本课作为门槛级课，需预设若干不确定因素。
简而言之，课程共分为三个阶段,
内容适用于最优秀的学生，但能够向下兼容，
作品在设计上逐步迭代。
基础较弱的学生能得到一个可运行的基础程序；
学有余力的学生可继续体验真实的软件开发过程，
包括对基础程序的扩展、重构和优化。

授课过程优先考虑完成项目，非必要实验可省略。

@handbook-scenario{阶段1:打字训练}

在此阶段，师生通过简单程序互相了解，
也包括安装开发工具、熟练使用键盘、
熟悉程序源码的基本结构、初步认识程序语言中的基本概念。

@tamer-figure!["d:shape" "计算思维之分解与合成"]{
 @stone-image["Disciplines/shape.png" #:scale 0.5]}

除了常规的 @litchar{Hello, World!} 开局，
我的课程更强调图形化和游戏化，
@tamer-figure-ref{d:shape}即是比较合适的开局例子。
在第三次或第四次课时，学生开始接触我的游戏引擎源码库，
并用代码绘制出这幅画作为外化的随堂练习。
六年级已经具备必要的几何学基础知识，
但未必具备必要的“素描构图”思维。

在该阶段结束时，学生会通过完成一个古老且简单的游戏来体验
@tech{自顶向下}的构建主义思路(即前文所说的“C++ 版 Scratch”)。
学有余力的学生可尝试在此游戏中的物体
和基本的智能体概念之间建立联系。

@handbook-scenario{阶段2:基础数据类型}

在此阶段，学生系统学习程序语言中的“原子”和“分子”。
即，基础数据类型，会比较多的强调跨学科应用。

@tamer-figure!["d:chromaticity" "色度图"]{
 @stone-image["Disciplines/chromaticity.png" #:scale figure-scale]}

计算机最擅长处理的数据类型显然是数字；
光是物理宇宙中最重要的自然现象之一，
显示器也是我们与数字世界沟通的最重要途径；
色彩本身也是图形化/游戏化的基础概念。
因此，本课程通过认识色彩、理解色彩的物理和数学定义来学习程序语言如何表达和处理数字。
色彩在不同模型中的表达方式不尽相同；
最常见的有RGB和HSV两大类，可分别用来学习定点数和浮点数。

系统地学习色彩科学难度极高。
在本课程中，浮点数部分是选修内容，
仅当学生定点数的色彩表示学得很好的情况下才会开设，
否则跳过，但是并不影响与后续课程的衔接。

@tamer-figure-ref{d:chromaticity}的色度图是一种用于衡量显示器成色能力的图。
在课堂上帮助教师解释“为什么显示器总会有失真”。学生作品是该图内容的子集。

在初步认识了数值类型之后，接下来就是计算机科学的基石类型：布尔类型与布尔代数。
在这里学生会系统接触逻辑运算和运算符优先级。
这应该是学生首次接触算术运算以外的运算，
不过必要的知识点，应该已经在语文课上有所涉及。

@tamer-figure!["d:drunkard" "计算思维之随机游走"]{
 @stone-image["Disciplines/drunkard.png" #:scale figure-scale]}

在该阶段结束时，学生通过创设情景来综合运用算术运算、逻辑运算与分支结构，
以简单处理(伪)随机数从而得到一系列看起来更自然或更实用的随机数(
@tamer-figure-ref{d:drunkard})。
对于能够理解分子的学生，
还可以增设一些物理实验来展示真随机现象，
或对比真随机现象和伪随机现象。

@racketcommentfont{这里还差一个阶段性项目，
 但该项目的定位是承上启下，
 必要性并不是特别高。}

@handbook-scenario{阶段3:初级计算思维}

此阶段会继续第二阶段的任务，学生开始学习线性数据结构：
高层的列表、底层的数组，以及基于数组的字符串。

列表和字符串都是一维数组的典型应用，
而二维数组的典型应用是网格。
于是，在随机游走的基础之上，
本阶段的第一个随堂练习就自然而然地过渡到“自回避游走”
(@tamer-figure-ref{d:self-avoiding.walk.png})了。
“自回避游走”有其实际应用场景。
比如，参观一座城市，记录自己的行动轨迹以确保不重复参观。
同时它也是课程作品的基础。

@tamer-figure!["d:self-avoiding.walk.png" "自回避游走"]{
 @stone-image["Disciplines/self-avoiding.walk.png" #:scale figure-scale]}

本阶段的主要目的是带学生完整体验真实的软件开发过程。
而从课程完整性角度来说，应该在基础课程中正式安排“流”相关概念。
因此，本阶段特别安排了一次课来重新认识标准输入输出和文件输入输出，
并配备了教师演示程序(也可供学生操作探索)以帮助学生直观理解
这个抽象概念(@tamer-figure-ref{d:stream})。
@tamer-figure-ref{d:stream} 中设计了两个管道
(“字节流”和“行缓冲流”)用于对比学习，
学生会发现左边的字节流管道不出汉字，
这时教师可根据需要引入“字符编码”话题。

@tamer-figure!["d:stream" "流"]{
 @stone-image["Disciplines/stream.png" #:scale figure-scale]}

在本课程作品中不一定会用到“流”知识(因为如何设计文件格式保存程序数据也值得单独
开一门课)。在后续课程，尤其是竞赛冲刺课程中，肯定有发挥空间。

从宇宙大爆炸到现在，最复杂的发展进程莫过于生命系统的演化。
因此，本课程的结课项目主题设定在生命科学范畴。
而生命科学的问题，很难归结为一条简单的数学公式，
因此通常会采用计算机大规模模拟的方式来研究，
这也正是计算思维最高级的应用：模拟和仿真。

@tamer-figure-ref{d:conway}和@tamer-figure-ref{d:evolution}
即是本课程的两个结课项目，学生根据自己的偏好任选其中一个来完成。
这两个项目的内核相同，都是通过计算机模拟的方式来探索生命演化现象。
差别在于，@tamer-figure-ref{d:conway}是数学家的做法，
极其抽象，如果有学生现在就能理解，好好培养将来必成大器；
@tamer-figure-ref{d:evolution}是自然科学家的做法，
比较符合真实的生态学研究。

@tamer-figure!["d:conway" "计算思维之数学仿真"]{
 @stone-image["Disciplines/conway.png" #:scale figure-scale]}

@tamer-figure-ref{d:conway}的“生命游戏”忠于原版。
这个项目除了过于抽象对想象力有较高要求外，其内部逻辑相当简单，
因此它更能体现应用程序的完整，包括用户交互操作、数据的加载和保存等。
学生除了能感受到“简单规则产生复杂行为”外，
还有较多时间来体验完整的应用程序开发过程。

现在有人已经证明了，可以在该游戏中模拟现代计算机。

@tamer-figure!["d:evolution" "计算思维之生态仿真"]{
 @stone-image["Disciplines/evolution.png" #:scale figure-scale]}

@tamer-figure-ref{d:evolution}的“演化”在原版的基础上增加了一些设定。
比如，繁殖周期。因为原版没有图形界面，只是简单展示世界的某一个快照，可以无限繁殖；
在我们的游戏界面下，无限繁殖必然会耗尽系统资源而导致程序奔溃
(你想象游戏中每秒60次指数级暴兵是个什么地狱景象)。

该程序内部逻辑比生命游戏复杂很多(但它的可探索性比生命游戏少很多)，
因此选做该项目的学生多半没时间在程序中加入交互操作，
也没时间设计文件格式保存和加载数据。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
