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

接下来以 @tt{basis} 目录为例说明课程内容组织（把一个软件假想为一只数字生物会更容易理解）。

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

以上条目未必都会出现在同一课程目录里，如果出现，一定符合上述解释。比如 @tt{literacy} 就只出现在 @tt{article} 里。

此外，还有两个特殊文件(夹)，文件名不可更改。

@itemlist[#:style 'compact
          @item{@deftech{info.rkt}: Racket 软件包的信息文件。用来配置我的 C++ 构建工具，比如哪些文件需要编译，怎么编译等。}
          @item{@deftech{compiled}: Racket 编译缓存文件目录，用于存放所有编译过程中可以自动生成的文件。以下几个子目录在我们的课程中也可能用到：
                 
           @itemlist[#:style 'compact
                     @item{@deftech{typesetting}: 存放 @tech{literacy}的输出，比如本手册 @filepath{compiled/typesettings/YouLanguage.pdf}。}
                     @item{@deftech{native}: 存放 C++ 二进制文件的输出，比如 @filepath{compiled/native/win32/x86_64/BigBang}。
                                 注意，它这里面会根据体系结构和操作系统划分子目录。}]}]

@tt{compiled} 和 Python 源码目录里的 @tt{__pycache__} 功能相同。

此外，Python 目录本身也有一系列约定俗成的规则，但目前我们的课程不会涉及相关概念，暂且不表。

@handbook-scenario[#:tag "wan_sync"]{广域网同步}

本系列所有课程的源码都组织在一个版本库(repository)里，其中共享源码(也就是前文一再提及的“配套的软件库”)以子库(submodule)的形式独立存在，以方便单独共享，
比如我自己的项目也会用到它。于是，初次同步，先选一个用于存放源码的目录，比如 @filepath{G:\Laboratory}，然后打开 PowerShell 执行以下命令：

@itemlist[#:style 'compact
          @commandline{git clone --recurse-submodules https://github.com/Gyoudmon/YouthLanguage.git G:\Laboratory\YouthLanguage}
          @commandline{cd G:\Laboratory\YouthLanguage}
          @commandline{git submodule foreach git checkout master}]

第三步很重要，要不然拉过来的子库源码不属于任何分支，这是 git submodule 经常被人诟病的地方。
以后需要更新时，只需

@itemlist[#:style 'compact
          @commandline{cd G:\Laboratory\YouthLanguage}
          @commandline{git submodule foreach git pull}
          @commandline{git pull}]

建议大家平时学一下 git 的基本用法，避免你不小心改了这的源码，然后 @exec{pull} 时冲突没法正常同步。
如果不想麻烦，那就删掉这个文件夹，从头开始重新 @exec{clone}。

@handbook-scenario[#:tag "lan_sync"]{局域网同步}

@handbook-scenario[#:tag "wisemon"]{C++ 的编译和运行}

在开发软件的过程中，打开 PowerShell 并且 @exec{cd} 到当前项目目录是个好习惯，本系列课程里，一般就是指有 @tech{info.rkt} 的目录。
如果涉及多个项目，每个项目单独开一个 PowerShell 比较合理，不用来回切换了。
如果使用 Visual Studio Code，也可以直接启动它的终端，不过那个终端是命令提示符，但在编译这件事上差别不大。
这样可以以假乱真，假装是在用 IDE。

对于 @tt{sketch} 这种草稿性质的代码，一般不需要特别复杂的编译参数，可以直接编译

@itemlist[#:style 'compact
          @commandline{raco wisemon -d cc [入口文件.cpp]}]

其中，@exec{raco} 是 Racket 提供的命令行；@exec{wisemon} 是我的构建工具；
@DFlag{-d} 表示输出编译过程，但不要太啰嗦；@exec{cc}是 C/C++ 编译器（C Compiler）的首字母缩写。

对于 @tt{basis} 这样的复杂项目，编译参数通常都会很复杂，因为我会在 @filepath{info.rkt} 里配置好，然后直接编译

@itemlist[#:style 'compact
          @commandline{raco wisemon -d cc}]

这次连带入口文件都不用指定，它会自己解析 @tech{info.rkt} 文件找到要编译的文件，并设置好相应的参数。
无论用哪种方式，编译完了之后的可执行文件，都在与入口文件相同目录的 @filepath{compiled/native} 子目录里。
比如， @tt{cpp/basis} 课程目录中有个 @filepath{FontBrowser.cpp} 文件，它已被配置在 @tech{info.rkt} 中。
于是，它对应的可执行文件名是@filepath{cpp/basis/compiled/native/win32/x86_64/FontBrowser.exe}。

Python 源码不需要编译，直接运行与 C++ 入口文件名相同的文件即可。

@handbook-scenario{学生目录}

一般来说，不建议学生直接修改教师的源码，学生应该另外找一个目录，保持目录结构不变，创建好自己的课程目录。
比如@filepath{G:\Course\YouthLanguage\cpp\basis}，然后把教师目录下的@filepath{info.rkt}
和已经配置好的入口文件复制到该目录下，最后单独@exec{clone}配套的源码库：

@itemlist[#:style 'compact
          @commandline{git clone https://github.com/Gyoudmon/digitama.cpp.git G:\Course\YouthLanguage\cpp\basis\digitama}]

Python 学生将其中的 @tt{digitama.cpp.git} 替换成 @tt{digitama.py.git} 即可：

@itemlist[#:style 'compact
          @commandline{git clone https://github.com/Gyoudmon/digitama.py.git G:\Course\YouthLanguage\python\basis\digitama}]

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
