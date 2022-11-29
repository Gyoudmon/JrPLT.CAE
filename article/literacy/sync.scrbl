#lang scribble/manual

@(require "literacy.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{源码库同步}

这是暂定方案，如有更方便的方法，随时更新。
教师之间推荐@seclink["wan_sync"]{广域网同步}方法，也可以像学生一样使用@seclink["lan_sync"]{局域网同步}方法。

@handbook-scenario{源码库结构}

做软件可以是一件比较有趣的事，教科书里的标准源码目录比较无趣，我的目录名字跟通用做法不一样。
因此，特别在这里说清楚，将来诸位也需要跟自己的学生说清楚。

这是目前的总目录：

@itemlist[#:style 'compact
          @item{@tt{cpp}: C++ 课程源码。}
          @item{@tt{python}: Python 课程源码。}
          @item{@tt{racket}: Racket 课程源码。这是我自己的私有课程，为我女儿和徒弟级学生准备。诸位有兴趣可以自行学习。}
          @item{@tt{article}: 这篇手册的源码。这可以作为例子告诉学生，编程和写作是同类活动；即使将来只想做作家，懂编程也将如虎添翼。}
          @item{@tt{utility}: 实用工具源码。这是我自己在研课过程中用的脚本，比如画出适合放在课件里的示意图。}]


接下来以 @tt{cpp} 目录为例说明课程目录组织，@tt{cpp} 和 @tt{racket} 同理。若无特殊情况，一门课只有一个根目录。

@itemlist[#:style 'compact
          @item{@tt{basis}: 语言基础课源码，这是所有后续课程的基础。除第一阶段，源码不会完整出现在课件里。目前的培训内容都在这里面。}
          @item{@tt{STEMA}: STEM 能力测试赛题源码。学生考前冲刺用的，也可作为非比赛学生夯实基本功的作业题。}
          @item{@tt{sketch}: 草稿源码，每个文件都自带 main，用于演示语言的某一个细节。比如众所周知的 @racket{Hello, World!}。
           这是个特殊的目录，@tt{basis}肯定会用到，其他课可能会用到，可以现场发给学生，也可以把源码包含在课件里。}]

接下来以 @tt{basis} 目录为例说明课程内容组织。

@itemlist[#:style 'compact
          @item{@deftech[#:key "digivice"]{.}: 源码根目录，相当于常规的 @tt{src}，存放最直接的源码，包括带 @italic{main} 的程序入口文件、
           你愿意共享给别人使用的模块文件和头文件。软件一旦发布，你修改这些文件必须保证兼容性，或以较为正式的方式告知别人为什么要放弃兼容性。
           顺便说一句，用我自己的构建工具编译 C++ 项目，可以存在多个 @italic{main}，一般一个 @italic{main} 代表一个程序。}
          @item{@deftech{digitama}: 私有源码目录，表示这个目录里的源码不对外开放。你修改这些文件无需保证兼容性，
           如果别人非要直接使用这些源码，后果自负。比如因你修改源码而导致他们程序崩溃，他们怨不得你。
           @tt{digitama} 是“数码蛋”的意思，意为我们的软件是从这个蛋里孵出来的。}
          @item{@deftech{stone}: 资源目录，相当于常规的 @tt{res} 或 @tt{Assets}，存放资源和(静态)配置信息，
           比如图片素材、界面文字的多语言翻译等。@tt{stone} 源自“罗赛塔石碑(Rosetta Stone)”，石碑上刻着对于任务而言至关重要的信息，
           这些信息只可读不可写。}
          @item{@deftech{tamer}: 测试目录，是常规的 @tt{tests} 和 @tt{docs} 的结合，用于存放测试程序和项目文档。
           这个更像是我的个人偏好，项目文档应当与软件的实际运行情况一致，因此一种做法是把文档以注释的形式写在源码里，
           但对于大项目仅仅因为修改了注释就要重新编译整个项目，这想想都不划算(大项目的编译相当耗费时间)。
           于是，我喜欢将文档和测试结合，这样我提供的文档，既包含了函数签名，还包含用法举例，以及算法的正确性验证等信息。
          @tt{tamer}是“驯兽师”的意思，寓意明显。}
          @item{@deftech{literacy}: 出版物源码目录。这也是我的个人偏好，在软件项目里没有对应的标准目录。相对于上面提到的项目文档，
           这个目录里面的内容要正式一些，作品可能是一本书、一篇论文等，以 pdf (而非网页)形式提供，比如你现在正在读的这份手册。
           @tt{literacy} 就是字面意思“文字能力”，无寓意。}
          @item{@deftech{village}: 协作目录，相当于开源项目的 @tt{contrib}。如果你的软件需要其他软件的源码，或者你想对其他软件提供支持，
           或者别人扩展了你的软件，这些源码都可以放在这里。@tt{village} 源自“创始村(Primary Village)”，数码宝贝出生的地方。}]

以上条目未必都会出现在同一课程目录里，但是如果出现，一定符合上述解释。比如 @tt{literacy} 就只出现在 @tt{article} 里，
用于存放手册源码。

此外，还有两个特殊文件(夹)，文件名不可更改。

@itemlist[#:style 'compact
          @item{@deftech{info.rkt}: Racket 软件包的信息文件。用来配置我的 C++ 构建工具，比如哪些文件需要编译，怎么编译等。}
          @item{@deftech{compiled}: Racket 编译缓存文件目录，用于存放所有编译过程中可以自动生成的文件。以下几个子目录在我们的课程中也可能用到：
                 
           @itemlist[#:style 'compact
                     @item{@deftech{typesetting}: 存放 @tech{literacy}的输出，比如本手册 @filepath{compiled/typesettings/YouLanguage.pdf}。}
                     @item{@deftech{native}: 存放 C++ 二进制文件的输出，比如 @filepath{compiled/native/x86_64-macosx/BigBang}。
                                 注意，它这里面会根据体系结构和操作系统划分子目录。}]}]

@tt{compiled} 和 Python 源码目录里的 @tt{__pycache__} 功能相同。

此外，Python 目录本身也有一系列约定俗成的规则，但目前我们的课程不会涉及相关概念，暂且不表。

@handbook-scenario[#:tag "wan_sync"]{广域网同步}

@handbook-scenario[#:tag "lan_sync"]{局域网同步}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
