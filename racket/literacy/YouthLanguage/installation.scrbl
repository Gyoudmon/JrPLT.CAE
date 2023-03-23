#lang scribble/manual

@(require "../literacy.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{配置开发环境}

@(define figure-scale 0.20)

@handbook-scenario{Racket}

Racket 是 Lisp 语系的一种方言，Lisp 是最古老的两大高级程序语言之一。
现在大多数人听说过的语言(包括 Python)的很多设计思路，基本都是 Racket 先有，
然后其他语言的作者一哄而上抄完就跑，吃相不可谓难看。

Racket 是我最常用的语言，它是现代 Lisp 最前卫的方言之一，也是“程序语言理论”领域最专业的语言(没有之一)，它老本行碰巧又是编程教育。
我们这个社区的活跃用户在全世界也就两千人，创始人是该领域的顶级计算机科学家，他们既希望自己的语言能得到大范围应用，
但又不希望过于大众，因为...那就庸俗了呀。

安装过程分三步。第一步简单，去@hyperlink["https://racket-lang.org/download/"]{官方网站}下载跟你系统一致的版本、安装即可，
后面两步分别各花一个小节来详细说明。

@handbook-action[#:tag "env-conf"]{配置环境变量}

环境变量是操作系统推送给每一个应用程序的特殊变量，用以告诉应用程序自己正运行在什么环境里，
比如系统是 Windows 还是 macOS、启动自己的用户是谁，等等。其中最重要的是，
程序知道要去哪里找别的程序以便在需要的时候协作完成一项任务。

命令提示符（cmd.exe）或 PowerShell 是两个最典型的程序，用户使用它们的大部分时间都在启动、执行别的程序。
当你随便输入一个不存在的命令名字时，它们会抱怨找不到这个程序。
但是现在，你输入 racket 它依然还是抱怨找不到，而你明明才刚刚安装上。
这是因为还没有配置环境变量，它们不知道去哪找到 racket。

对于我们用得到的部分，cmd.exe 和 PowerShell 差不多。
但方便起见，建议大家熟练使用 PowerShell 常用命令。
接下来开始配置环境变量：

@itemlist[#:style 'compact
          @item{在 PowerShell 里输入 @exec{sysdm.cpl} 回车打开@tamer-figure-ref{sysdm}的“系统属性”窗口。
           @tamer-figure-here["sysdm" "系统属性"]{@stone-image["installation/sysdm_cpl.png" #:scale figure-scale]}}

          @item{在“高级”标签页最下方有个“环境变量”，点击打开 @tamer-figure-ref{env}。
           @tamer-figure-here["env" "环境变量"]{@stone-image["installation/env.png" #:scale figure-scale]}}
          
          @item{编辑“系统变量”里的 @envvar{Path}，对照@tamer-figure-ref{path}，把 @filepath{C:\Program Files\Racket} 加进去。
           越靠上的路径优先级越高。@tamer-figure-here["path" "编辑环境变量"]{@stone-image["installation/path.png" #:scale 0.36]}}]

至此，关闭 PowerShell 再重新打开就可以运行 racket 命令了。
可以用@exec{where.exe}（后缀@exec{.exe}不能省略）命令来确定某个程序能不能被找到，
比如：

@itemlist[#:style 'compact
          @commandline{where.exe racket}]

这个命令会告诉你 racket 存放的完整路径，如果你安装了多个版本，它也会把其他版本的路径都列出来，默认情况运行第一个。

@centered{@bold{环境变量对于软件工程师来说很重要，后续安装过程中还会多次往@envvar{Path}里面加东西。}}

@handbook-action{安装我写的构建工具}

Racket 在本系列课程中充当 C++ 构建工具，一行简短的命令搞定所有恼人的编译细节，特别是解决了“一个项目只能有一个main”问题。
在 PowerShell 里运行如下命令：

@itemlist[#:style 'compact
          @commandline{raco pkg install -i --auto digimon}]

详细的构建过程见@Secref{wisemon}。

下面这几个软件包可能对大家有点用，如果感兴趣可以继续安装，没兴趣就跳过。
这些工具用得好可以提高研课效率，但肯定需要额外花时间去学。

@itemlist[#:style 'compact
          @commandline{raco pkg install -i --auto graphics w3s mox}]

最后说一句，用我自己写的软件来教学生，肯定有家长会质疑这个做法。
嗯，初学编程真的麻烦且枯燥，如果你不希望你的孩子在第一个月就被各种烦人的细节劝退，那就放宽心。
先入门激活了兴趣再考虑别的，比如竞赛组学生会专门培训标准工具链以应付考试。
再说了，我的代码质量经过真实世界的检验，你觉得亏的可能性只有一种：格局低了。

悄悄地说一声，你可以在 Racket 某些版本的致谢名单里看到我的英文名（嘘）。

@handbook-scenario{git}

@tamer-figure-here["git_path" "git 环境变量"]{@stone-image["installation/git_path.png" #:scale 0.42]}
git 是目前应用最广泛的版本管理工具。建议大家有空了熟悉一下 git 的基本用法。

在本系列课程中，git 既用于管理我们自己的代码库，也用于管理和安装依赖的第三方库。
安装过程不算复杂，去@hyperlink["https://git-scm.com/download/win"]{官方网站}下载自己系统的版本。
但在一路@onscreen{下一步}的过程中，注意@tamer-figure-ref{git_path}界面，是给 git 配置
@envvar{Path} 环境变量的，它默认选了第二项，但我建议大家选第三项。那个可以让大家在 PowerShell
里使用 Linux 命令，比 Windows 自己的同名命令好用一些。后续课程里针对竞赛班的培训，命令行肯定是以
Linux 的为准。实际上，很多好用的工具都没有 Windows 版，移植到 Windows 上去的时候都会自带一个微型的
Linux 环境。这也是为什么明明是个很小的工具，安装起来却动辄几百兆的原因。

如果已经安装了 git，但没有选第三项的，只需回顾@seclink["env-conf"]{环境变量}，
把以下两个路径也加到 @envvar{Path} 里，顺序不要错：

@itemlist[#:style 'compact
          @item{@filepath{C:\Program Files\Git\mingw64\bin}}
          @item{@filepath{C:\Program Files\Git\usr\bin}}]

完毕。

@handbook-scenario{SDL2}

SDL2 是 Simple DirectMedia Layer 2.0，一个跨平台的 2D 游戏引擎，也是 Python 的游戏库也以它为基石。

@centered{@bold{本节可以跳过，课程源码库已经自带 SDL2。但建议教师熟悉本节流程。}}

在本系列课程中，SDL2 是 C++ 和 Python 代码库的底层接口，用于显示窗口并在里面播放动画或运行游戏，
不需要下载源码，需要用操作系统的软件包管理工具(不同于软件商店)安装二进制动态链接库。

@handbook-action[#:tag "vcpkg"]{Windows 下使用 vcpkg 安装 SDL2}

vcpkg 是微软自己的软件包管理工具，用于安装与自己的编译器兼容的第三方软件库，其中就包括我们需要的 SDL2。

vcpkg 没有现成的可执行文件，而是直接提供源码用 git 来安装和升级。接下来的步骤需要在 PowerShell 里运行（每个“--”引导的行都是一条命令）：

@itemlist[#:style 'compact
          @item{下载 vcpkg 并安装在 @filepath{C:\opt\vcpkg} 目录里。
                 @itemlist[#:style 'compact
                           @commandline{git clone https://github.com/microsoft/vcpkg C:\opt\vcpkg}
                           @commandline{C:\opt\vcpkg\bootstrap-vcpkg.bat}]}

          @item{安装 64bit SDL2
                 @itemlist[#:style 'compact
                           @commandline{C:\opt\vcpkg\vcpkg install sdl2 sdl2-ttf sdl2-image sdl2-gfx sdl2-mixer sdl2-net --triplet=x64-windows}]}
          
          @item{用管理员身份运行(普通用户运行时可现场授权，无需另开终端)
                 @itemlist[#:style 'compact
                           @commandline{C:\opt\vcpkg\vcpkg integrate install}]}
          
          @item{回顾@seclink["env-conf"]{环境变量}，将@filepath{C:\opt\vcpkg}和@filepath{C:\opt\vcpkg\installed\x64-windows\bin}两项分别加到环境变量@envvar{Path}中。}]

完毕。

@handbook-scenario{Visual Studio}

@tamer-figure-here["vs" "安装 Visual Studio（含 C++ 和 Python）"]{@stone-image["installation/vs.jpg" #:scale 0.32]}
Visual Studio 是微软自己的标准IDE(集成开发环境，包括编辑、编译、合作等一系列专业软件开发的工作流)。

@centered{@bold{如果你不想安装如此庞大的 IDE，可以跳到@seclink["vc_BuildTools"]{vs_BuildTools}，安装纯命令行工具。}}

对学生而言 Visual Studio 可能过于庞杂了，但在本系列课程中，安装它主要是为了跟它一起安装的 C++ 编译器。
去@hyperlink["https://visualstudio.microsoft.com/downloads/"]{官方网站}下载免费的 Community 版，然后对照
@tamer-figure-ref{vs}至少勾选@onscreen{Desktop Development with C++}一项。如果你想在 Visual Studio 里折腾
Python，可以再勾选@onscreen{Python Development}，然后在右侧的@onscreen{optional}栏里第一项手动勾选 Python 解释器。
安装过程比较漫长，耐心等待即可。之后回顾@seclink["env-conf"]{环境变量}，
将@filepath{C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build}加进@envvar{Path}里去。
将来 Visual Studio 出新版本了不会影响这个2022版的，除非你自己把它删了。

@handbook-action[#:tag "vc_BuildTools"]{仅安装 vs_BuildTools}

@tamer-figure-here["vsbt" "安装 VS Build Tools"]{@stone-image["installation/vs_buildtools.png" #:scale 0.24]}
安装 Visual Studio 时会自动安装一份 vs_BuildTools，但其实这套工具也可以单独安装，可以省下不少硬盘空间（因为它不提供图形界面）。
对于本系列课程，Visual Studio 并不是必须的。

直接点这个@hyperlink["https://aka.ms/vs/17/release/vs_BuildTools.exe"]{隐藏连接}下载。
注意，它的安装界面跟 Visual Studio 极其相似，所以别被骗了，或觉得自己下载错了。

一切从简，只需勾选@onscreen{Desktop Development with C++}一项即可，
并同时去掉@tamer-figure-ref{vsbt}右侧@onscreen{optional}栏除第一项以外的所有选项。
这次安装过程不长，之后回顾@seclink["env-conf"]{环境变量}，
将@filepath{C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build}加进@envvar{Path}里去。
注意，这个路径跟 Visual Studio 自带的那个不一样。

这个工具实际上也能在@hyperlink["https://visualstudio.microsoft.com/downloads/"]{官方下载页面}找到，
只不过它默认被折叠了。拉到最下面，点开@onscreen{Tools for Visual Studio}，找到@onscreen{Build Tools for Visual Studio 2022}，
后面那个@onscreen{Download}按钮就是。以后Visual Studio 更新了，这个连接的名字也会跟着变的，到时重新安装即可。

@handbook-scenario{Visual Studio Code}

Visual Studio Code 也是微软推出的产品，但它主要职能是“编辑”代码，功能比 Visual Studio 少很多。
Visual Studio Code 是开源软件，别人可以一起贡献代码，因而安装了合适扩展就可以编辑所有语言的代码，
且字体、配色、操作风格一致，这一点 Visual Studio 是做不到的。
对学生而言 Visual Studio Code 足够轻便，也有利于他们分清楚“编辑”和“编译”两个过程。

它的安装更为简单，还是去同一个@hyperlink["https://visualstudio.microsoft.com/downloads/"]{页面}找到那个大大的
Visual Studio Code，下载安装一气呵成。

之后，当你打开一个明显是代码的文件时，它会提醒你安装相应语言的扩展，然后就可以愉快地编码了。要运行 C++ 有点麻烦，可能是因为微软的编译器
依赖一大堆环境变量，而这些变量又随时可能变化，普遍程序员的配置成本太高了。前面我们配置的那个环境变量并不能直接运行 C++ 编译器，
但里面有得到所有环境变量的程序。详细构建过程见@Secref{wisemon}。

@handbook-scenario{Python}

Python 是什么不需要我再强调了，地球人都知道，不过它也就“用的人多”这一个优点。

虽然安装 Visual Studio 会同时安装一个 Python，不过我还是建议自己额外安装，最起码版本比较新。
而且，现在在 PowerShell 里已经有 @exec{python.exe} 了，这是一个空壳，会引导去商店安装，不过我这商店一直无法联网，不折腾了。

去@hyperlink["https://www.python.org/downloads/windows/"]{官方网站}下载与你的体系结构相对应的
@onscreen{installer}，现在一般都是@tt{64-bit}的。注意，一定要是@onscreen{installer}，不能是@onscreen{embeddable package}。

@tamer-figure-here["python" "安装 Python"]{@stone-image["installation/python.png" #:scale figure-scale]}
安装过程有个细节，务必在安装界面勾选@onscreen{add python.exe to PATH}(@tamer-figure-ref{python})。
这样，一来你自己不用再折腾了，二来它会帮你压制 Windows 自带的那个商店壳子。

需要注意的是，就因为 Python 用的人多，导致一个系统里好多互补兼容的版本并存。
于是，在自己安装 Python 的 Windows 里，运行 Python 程序只需启动 @exec{python} 就行了，
其他情况有可能需要启动@exec{python3}。Windows自带的那个@exec{python3.exe}现在仍为商店空壳。
我的课件里也会强调@exec{python3}。

你也可以按自己的喜好安装其他 Python IDE，比如 PyCharm 之类的。

@handbook-action{安装 PySDL2}

PySDL2 是 Python 对 SDL2 的封装。

在本系列课程中，PySDL2 是课程代码库和SDL2的中间层。也就是说，只有装了这个，
Python 才跟 C++ 处于同一起跑线上，我的代码库的 Python 版就从这里开始。

保险起见，还是要先关闭当前 PowerShell 再重新打开，使之前配置的 @envvar{Path} 生效。
执行以下命令安装 PySDL2

@itemlist[#:style 'compact
          @commandline{python -m pip install pysdl2 pysdl2-dll pygame}]

那个 PyGame 是 Python 自己提供的建立在老版 SDL 基础之上的游戏引擎。
按理说，不装它也没有问题，但实际情况是不装它我们的程序也运行不起来。
大概是这几个软件包的依赖关系比较混乱，凑合着用吧。

@handbook-scenario{Scratch}

目前 Scratch 无需特别配置，安装完开箱即用。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
