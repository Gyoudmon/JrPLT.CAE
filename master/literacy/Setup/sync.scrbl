#lang scribble/manual

@(require "../literacy.rkt")

@require{../graphviz.rkt}

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{源码库同步}

这是暂定方案，如有更方便的方法，随时更新。
教师之间推荐@seclink["wan_sync"]{广域网完整同步}方法，
也可以像学生一样使用@seclink["student_workspace"]{学生目录}法。

@handbook-scenario{源码库结构}

做软件可以是一件比较有趣的事，教科书里的标准源码目录比较无趣，我的目录名字跟通用做法不一样。
因此，特别在这里说清楚，将来诸位也需要跟自己的学生说清楚。

这是目前的总目录：

@itemlist[#:style 'compact
          @item{@tt{cpp}: C++ 课程源码。}
          @item{@tt{python}: Python 课程源码。}
          @item{@tt{master}: 学会编程能做的事情远远多于做软件。
           这个目录里包含所有的其他源码： 课程文档源码(比如你正在读的这篇)、
           生成课件图片素材的源码、课程宣传用项目等等。}
          @item{@tt{mascot}: 课程素材，主要是图片。所有语言通用。}]

对照@tamer-figure-ref{srctree}，
每个课程的源码按如下目录组织（先把课程假想成软件，再把软件假想成数字生物，这样会更容易理解）。

@itemlist[#:style 'compact
          @item{@tamer-deftech[#:key "digivice"]{.}: 源码根目录，相当于常规的 @tt{src}。
           这里存放最直接的源码，包括带 @italic{main} 的程序入口文件、
           你愿意共享给别人使用的模块文件和头文件。
           软件一旦发布，你修改这些文件必须保证兼容性，
           或以较为正式的方式告知别人为什么要放弃兼容性。
           顺便说一句，用我自己的构建工具编译 C++ 项目，
           可以存在多个 @italic{main}，一般一个 @italic{main} 代表一个程序。}
          @item{@tamer-deftech{digitama}: 私有源码目录，表示这个目录里的源码不对外开放。
           你修改这些文件无需保证兼容性，如果别人非要直接使用这些源码，后果自负。
           比如因你修改源码而导致他们程序崩溃，他们怨不得你。
           @tt{digitama} 是“数码蛋”的意思，意为我们的软件是从这个蛋里孵出来的。}
          @item{@tamer-deftech{stone}: 资源目录，相当于常规的 @tt{res} 或 @tt{Assets}。
           这里存放资源和(静态)配置信息，
           比如图片素材、界面文字的多语言翻译等。
           @tt{stone} 源自“罗赛塔石碑(Rosetta Stone)”，
           石碑上刻着对于任务而言至关重要的信息，这些信息只可读不可写。}
          @item{@tamer-deftech{tamer}: 测试目录，是常规的 @tt{tests} 和 @tt{docs} 的结合。
           这里用于存放测试程序和项目文档。
           这个更像是我的个人偏好，项目文档应当与软件的实际运行情况一致，
           因此一种做法是把文档以注释的形式写在源码里，
           但对于大项目仅仅因为修改了注释就要重新编译整个项目，
           这想想都不划算(大项目的编译相当耗费时间)。
           于是，我喜欢将文档和测试结合，这样我提供的文档，
           既包含了函数签名，还包含用法举例，以及算法的正确性验证等信息。
          @tt{tamer}是“驯兽师”的意思，寓意明显。}
          @item{@tamer-deftech{literacy}: 出版物源码目录。
           这也是我的个人偏好，在软件项目里没有对应的标准目录。
           相对于上面提到的项目文档，这个目录里面的内容要正式一些，
           作品可能是一本书、一篇论文等，
           以 pdf (而非网页)形式提供，比如你现在正在读的这份手册。
           @tt{literacy} 就是字面意思“文字能力”，无寓意。}
          @item{@tamer-deftech{village}: 协作目录，相当于开源项目的 @tt{contrib}。
           总体上说，这个目录里面的东西比较包罗万象。
           比如，你的软件用到了其他软件、你想编写额外代码以支持其他软件、
           别人与你协作开发并扩展了你的软件，
           这些源码（或二进制文件）都可以放在这里。
           @tt{village} 源自“创始村(Primary Village)”，数码宝贝出生的地方。

           
           在本系列课程中，其他类型的课程代码也放在这里了。比如
           @itemlist[#:style 'compact
                     @item{@tt{STEMA}: STEM 能力测试赛题源码。
                                 学生考前冲刺用的，也可作为非比赛学生夯实基本功的作业题。}
                     @item{@tt{sketch}: 草稿源码，每个文件都自带 main，用于演示语言的某一个细节。
                                 比如众所周知的 @litchar{Hello, World!}。
                                 这是个特殊的目录，@tt{basis}肯定会用到，其他课可能会用到，
                                 可以现场发给学生，也可以把源码包含在课件里。}
                     @item{@tt{gv}: 软件工程图表相关 Graphviz 源码。}
                     @item{@tt{procedural}: 过程式风格示例源码。
                                 过程式风格更为注重细节，复杂度未必有多大，但代码量多半会让初学者生畏。
                                 因此仅作为对比材料发放给学生，学生根据自身情况课后自行决定是否掌握。}]}]

以上条目未必都会出现在同一课程目录里，如果出现，一定符合上述解释。
比如 @tt{literacy} 就只出现在 @tt{master} 里，手册只需写一份就行。

@tamer-figure["srctree" "源码目录结构"]{
 @filesystem-tree[
 '(|.|
   (info.rkt . "Racket 软件包元信息文件")
   ((digitama . "私有源码")
    (big_bang . "游戏内核源码"))
   (stone . "资源目录")
   (literacy . "出版物源码")
   ((village . "协作目录")
    (gv . "Graphviz 源码")
    (procedural . "过程式风格示例代码")
    (sketch . "课堂练习用草稿源码")
    (STEMA . "STEM 能力测试题库"))
   ((compiled . "Racket 编译缓存目录")
    (typesetting . "含本手册成品")
    (native . "含 C++ 可执行文件")))]}

此外，还有两个特殊文件(夹)，文件名不可更改。

@itemlist[#:style 'compact
          @item{@tamer-deftech{info.rkt}: Racket 软件包的元信息文件。
           用来配置我的 C++ 构建工具，比如哪些文件需要编译，怎么编译等。}
          @item{@tamer-deftech{compiled}: Racket 编译缓存目录。
           用于存放所有编译过程中可以自动生成的文件。
           注意，此目录不唯一，各个被编译的文件所在的目录里都有一个。

           以下几个子目录在我们的课程中也可能用到：      
           @itemlist[#:style 'compact
                     @item{@tamer-deftech{typesetting}: 存放 @tech{literacy}的输出。
                                 比如本手册 @filepath{literacy/compiled/typesetting/YouLanguage.pdf}。}
                     @item{@tamer-deftech{native}: 存放 C++ 二进制文件的输出。
                                 比如 @filepath{compiled/native/BigBang}。}]}]

@tt{compiled} 和 Python 源码目录里的 @tt{__pycache__} 功能相同。

此外，Python 目录本身也有一系列约定俗成的规则，
但目前我们的课程不会涉及相关概念，暂且不表。

@handbook-scenario[#:tag "wan_sync"]{广域网完整同步}

本系列所有课程的源码都组织在一个版本库(repository)里，
其中共享源码(也就是前文一再提及的“配套的软件库”)以子库(submodule)的形式独立存在，以方便单独共享。
于是，初次同步，先选一个用于存放源码的目录，比如 @filepath{G:\Laboratory}，
然后打开 PowerShell 执行以下命令：

@itemlist[#:style 'compact
          @commandline{cd G:\Laboratory\YouthLanguage}
          @commandline{git clone --recurse-submodules https://github.com/Gyoudmon/YouthLanguage.git .}
          @commandline{git submodule foreach git checkout master}]

第三步很重要，要不然拉过来的子库源码不属于任何分支，
这是 git submodule 经常被人诟病的地方。
以后需要更新时，只需

@itemlist[#:style 'compact
          @commandline{cd G:\Laboratory\YouthLanguage}
          @commandline{git pull}
          @commandline{git submodule foreach git pull}]

建议大家平时学一下 git 的基本用法，
避免不小心修改了拉回来的代码，
导致下次 @exec{pull} 时冲突没法正常同步。
如果真碰到这事了，又嫌学 git 麻烦，
那就删掉这个文件夹，从头开始重新 @exec{clone}。

@handbook-scenario[#:tag "wisemon"]{C++ 代码的编译和运行}

在开发软件的过程中，打开 PowerShell 并且 @exec{cd} 到当前项目目录是个好习惯。
在本系列课程中，一般就是指 @tech{info.rkt} 文件所在的目录。
如果涉及多个项目，每个项目单独开一个 PowerShell 比较合理，不用来回切换了。
如果使用 Visual Studio Code，本课程源码自带运行和调试配置，无需额外折腾。

对于 @tt{sketch} 这种草稿性质的代码，
一般不需要特别复杂的编译参数，可以直接编译

@itemlist[#:style 'compact
          @commandline{raco wisemon -d cc [入口文件.cpp]}]

其中，@exec{raco} 是 Racket 提供的命令行；
@exec{wisemon} 是我的构建工具；
@DFlag{-d} 表示输出编译过程，但不要太啰嗦；
@exec{cc}是 C/C++ 编译器（C Compiler）的首字母缩写。

对于 @tt{basis} 这样的复杂项目，编译参数通常都会很复杂，
因为我会在 @filepath{info.rkt} 里配置好，然后直接编译

@itemlist[#:style 'compact
          @commandline{raco wisemon -d cc}]

这次连入口文件都不用指定，
它会自己解析 @tech{info.rkt} 文件找到要编译的文件，
并设置好相应的参数。
无论用哪种方式，编译完了之后的可执行文件，
都在与入口文件相同目录的 @filepath{compiled/native} 子目录里。
比如， @tech{info.rkt} 中已经配置了 @filepath{cpp/FontBrowser.cpp}，
它在 Windows 下对应的可执行文件名是@filepath{cpp/compiled/native/FontBrowser.exe}。

Python 源码不需要编译，直接运行相应的入口文件即可。
但需要注意的是，由于 Python 奇葩的模块管理规则，
入口模块应当创建在项目目录下，
否则它会罢工并抱怨“你在用撞大运的方式编程”
(Python的作者是个走了狗屎运的普信男)。

@handbook-scenario[#:tag "student_workspace"]{学生目录}

一般来说，不建议学生直接修改教师的源码，
学生应该另外找一个目录，保持目录结构不变，创建好自己的课程目录。
比如@filepath{G:\Course\YouthLanguage}，
然后把教师目录下的@filepath{info.rkt}
和已经配置好的入口文件复制到该目录下，
最后单独@exec{clone}配套的源码库和素材库：

@itemlist[#:style 'compact
          @commandline{cd G:\Course\YouthLanguage}
          @commandline{git clone https://github.com/Gyoudmon/big-bang.cpp.git cpp\digitama\big_bang}
          @commandline{git clone https://github.com/Gyoudmon/mascot.git mascot}]

使用 Windows 的学生还需要再同步一个@tt{vcso.git}。
有这个就不用另外安装 vcpkg 了（就是那个可以跳过的@Secref{vcpkg}）。
使用其他操作系统的学生无需同步这个共享库。

@itemlist[#:style 'compact
          @commandline{git clone https://github.com/Gyoudmon/vcso.git C:\opt\vcso}]

@bold{注意，@tt{vcso.git} 的安装位置不建议修改。}

Python 学生将其中的 @tt{big-bang.cpp.git} 替换成 @tt{big-bang.py.git} 即可，
也不需要 @tt{vcso.git}：

@itemlist[#:style 'compact
          @commandline{cd G:\Course\YouthLanguage}
          @commandline{git clone https://github.com/Gyoudmon/big-bang.py.git python\digitama\big_bang}
          @commandline{git clone https://github.com/Gyoudmon/mascot.git mascot}]

@handbook-scenario[#:tag "nanny_sync"]{保姆级同步}

实际上，因为一些众所周知的原因，
学生的个人电脑大概率不能正常访问 GitHub。
因此，教师需要在自己的电脑上(或者配个树莓派)搭建一个简单的 git 服务器，
详细过程请自行搜索资料学习（最简单的方案无需安装额外软件，
但要先启动 OpenSSH Server 服务。
OpenSSH 是时下应用比较广泛的加密隧道，
很多需要安全远程登陆又不想自己做相关功能的软件都会搭它的顺风车）。

新建一个“允许通过 SSH 远程登陆”的系统用户，
比如@litchar{stem}(密码与用户名相同)。
然后，在该用户的HOME目录下创建一系列用于共享给学生的 git 裸库
(不带工作目录的库)。
学生机器上只需安装 git 即可。

目前需要自行维护的裸库有:

@itemlist[#:style 'compact
          @item{@tt{digimon.git}: 我的 C++ 构建工具}
          @item{@tt{basis.git}: C++ 版语言基础课的作品源码}
          @item{@tt{basis.py.git}: Python 版语言基础课的作品源码}
          @item{@tt{big-bang.git}: C++ 版游戏引擎源码}
          @item{@tt{big-bang.py.git}: Python 版游戏引擎源码}
          @item{@tt{mascot.git}: 游戏引擎自带图片素材}
          @item{@tt{vcso.git}: Windows 必要的动态链接库}]

现以 @tt{digimon.git} 为例说明管理裸库的一般步骤
(命令行中的变量用红色底纹装饰)。

远程登陆@litchar{stem}账号，
此时的“当前目录”就是该账号的HOME目录。
简单起见不要切换到其他目录，
然后执行：

@itemlist[#:style 'compact
          @commandline{git init --bare @litchar{digimon}.git}]

创建好所有裸库之后，就不用再管服务器账号了。
对于新创建的裸库，还需在对应的开发库里添加一个名为
@litchar{student}的@tt{remote}地址：

@itemlist[#:style 'compact
          @commandline{git remote add student @litchar{stem}@string[#\@]@litchar{IP}@string[#\:]@litchar{digimon}.git}]

@tt{remote}地址可以写成任何标准网络地址格式。
而 git 搭了 SSH 的顺风车，
因此这个地址也可以写成 SSH 的地址格式：
@racketresultfont{用户名@string[#\@]主机地址:相对于用户首目录的路径}。
以后，如源码有所变动，除了完成你自己惯例要做的事外，
还需要额外将变动推送给学生裸库(不要加@Flag{u}选项，
否则学生裸库就会变成你的默认上游)：

@itemlist[#:style 'compact
          @commandline{git push student}]

学生克隆/拉取最新源码也用上述 SSH 格式的地址。
新学员配置好环境变量@envvar{Path}后，
在 PowerShell 中按顺序执行以下步骤：

@itemlist[#:style 'compact

          @item{克隆辅助软件源码和资源库：
           @itemlist[#:style 'compact
                     @commandline{git clone @litchar{stem}@string[#\@]@litchar{IP}@string[#\:]@litchar{digimon}.git C:\opt\digimon}
                     @commandline{git clone @litchar{stem}@string[#\@]@litchar{IP}@string[#\:]@litchar{mascot}.git C:\opt\mascot}
                     @commandline{git clone @litchar{stem}@string[#\@]@litchar{IP}@string[#\:]@litchar{vcso}.git C:\opt\vcso}]}
          
          @item{安装构建软件：
           @itemlist[#:style 'compact
                     @commandline{raco pkg install -i --auto --link C:\opt\digimon}]}]

最后，对于每一个具体课程的源码库，
教师需要精细控制源码库的内容，包括但不限于：

@itemlist[#:style 'compact
          @para{去掉开发库中的所有子库(@emph{submodule})，但保持目录结构不变。
           子库可以以源码形式提供，也可以以动态链接库的形式提供。
           前者方便学生自行研究源码，后者便于学生保持工作目录清爽。}
          @para{修改学生程序的配置选项，使得构建程序和项目作品知道去@filepath{C:\opt}找资源。}]

建议学生克隆两份，一份用作范例源码，仅接受更新不做修改；
另一份用作正常课程的项目目录，
但为方便学生在学习过程中对比范例源码和自己的源码，
仍需保留工作目录中的范例代码，
并在合适的路径上创建自己的作品文件。
比如，在@tamer-figure-ref{stutree}中，
@tt{basis}和@tt{NAME}都是学生源码目录，
前者是范例，后者则需要学生自己在学习过程中逐渐完善。
同时，对于学生作品的入口文件，可以如法炮制，
也可以直接修改，总之保证导入的模块都是学生自己的即可。

@tamer-figure["stutree" "学生工作目录结构(片段)"]{
 @filesystem-tree[
 '(basis
   (digitama
    (big_bang . "游戏内核源码")
    (basis . "教师的基础课程源码")
    (NAME . "学生[NAME]的基础课程源码")))]}

于是，如果课程源码库已经包含子库源码，学生只需一步克隆：

@itemlist[#:style 'compact
          @commandline{git clone @litchar{stem}@string[#\@]@litchar{IP}@string[#\:]@litchar{basis}.git [学生工作总目录]}]

如果没有包含，学生又想要子库源码，那就按需继续克隆：

@itemlist[#:style 'compact
          @commandline{git clone @litchar{stem}@string[#\@]@litchar{IP}@string[#\:]@litchar{basis}.git [学生工作总目录]/digitama/big_bang}]

关于这个“学生工作总目录”，不要问学生选哪个了，
因为学生(和家长)大概率都还没有相关意识。
直接给建议，
比如在D盘(或其他非系统盘)根目录下创建一个以学生的英文名命名的目录
(@racketerror{目录名一定不能带中文和空白字符})。

对于学生个人项目，建议移除@tt{remote}地址，
或者干脆直接删掉@filepath{.git}配置目录。
对于团队项目，教师需要在服务器端专门为每个团队每个项目新建一个裸库，
方便团队成员之间共享各自负责的模块。

终于写完了，青少年学C++第零难：搞定工作环境。
这个世界上开发软件众多，但竟然没有一个对青少年友好，
即使它们的目标受众是学生。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
