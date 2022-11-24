#lang scribble/manual

@(require "literacy.rkt")

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{配置开发环境}

@bold{注意，建议诸位在自己的电脑里备好梯子。
这样不仅本次安装过程无硬伤，后续我们内部协作也会顺畅很多。}

@(define figure-scale 0.20)

@handbook-scenario{git}

git 是目前应用最广泛的版本管理工具。建议大家有空了熟悉一下 git 的基本用法。

在本系列课程中，git 既用于管理我们自己的代码库，也用于管理和安装依赖的第三方库。
这个安装比较简单，去@hyperlink["https://git-scm.com/download/win"]{官方网站}下载自己系统的版本即可。

@handbook-action[#:tag "env-conf"]{环境变量}

环境变量是操作系统推送给每一个应用程序的特殊变量，用以告诉应用程序自己正运行在什么环境里，
比如系统是 Windows 还是 macOS、启动自己的用户是谁，等等。其中最重要的是，
程序知道要去哪里找别的程序以便在需要的时候协作完成一项任务。

命令提示符或 PowerShell 是两个最典型的程序，用户使用它们的大部分时间都在启动、执行别的程序。
当你随便输入一个不存在的命令名字时，它们会抱怨找不到这个程序。但是现在，你输入 git 它依然还是抱怨找不到，
而你明明才刚刚安装上。这是因为还没有配置环境变量，它们不知道去哪找到 git。

@itemlist[#:style 'compact
          @item{按 @exec{Win+R} 快捷键打开 Run，输入 @exec{sysdm.cpl} 回车打开@tamer-figure-ref{sysdm}的“系统属性”窗口。
                  @tamer-figure-here["sysdm" "系统属性"]{@image["stone/installation/sysdm_cpl.png" #:scale figure-scale]}}

          @item{在“高级”标签页最下方有个“环境变量”，点击打开 @tamer-figure-ref{env}。
                  @tamer-figure-here["env" "环境变量"]{@image["stone/installation/env.png" #:scale figure-scale]}}

          @item{编辑“系统变量”里的 @envvar{Path}，对照@tamer-figure-ref{path}，检查是否有 git 项，如果没有就新建一个，把
                @filepath{C:\Program Files\Git\cmd} 加进去。
                @tamer-figure-here["path" "编辑环境变量"]{@image["stone/installation/path.png" #:scale figure-scale]}}]

至此，关闭 PowerShell 再重新打开就可以运行 git 命令了。可以用@exec{where.exe}（后缀@exec{.exe}不能省略）命令来确定某个程序能不能被找到：

@itemlist[#:style 'compact
          @commandline{where.exe git}]

这个命令会告诉你 git 存放的完整路径。

另外，修改环境变量的过程对于软件工程师来说很重要，后续安装过程中还会多次往@envvar{Path}里面加东西。


@handbook-scenario{Racket}

Racket 是 Lisp 语系的一种方言，Lisp 是最古老的两大高级程序语言之一。
现在大多数人听说过的语言(包括 Python)的很多设计思路，基本都是 Racket 先有，
然后其他语言的作者一哄而上抄完就跑，吃相不可谓难看。

Racket 是我最常用的语言，它是现代 Lisp 最前卫的方言之一，也是“程序语言理论”领域最专业的语言(没有之一)，它老本行碰巧又是编程教育。
我们这个社区的活跃用户在全世界也就两千人，创始人是该领域的顶级计算机科学家，他们既希望自己的语言能得到大范围应用，
但又不希望过于大众，因为...那就庸俗了呀。

Racket 在本系列课程中充当 C++ 构建工具，一行简短的命令搞定所有恼人的编译细节。
此外，我自己研课时也经常用它写小程序以提高工作效率，包括写出这篇文档。

顺便说一句，用我自己写的软件来教学生，肯定有家长会质疑这个做法。
嗯，初学编程真的麻烦且枯燥，如果你不希望你的孩子在第一个月就被各种烦人的细节劝退，那就放宽心。
先入门激活了兴趣再考虑别的，比如竞赛组学生会专门培训标准工具链以应付考试。
再说了，我的代码质量经过真实世界的检验，你觉得亏的可能性只有一种：格局低了。

悄悄说一声，你可以在 Racket 某些版本的贡献者列表里看到我的英文名（嘘）。

安装过程分三步：

@itemlist[#:style 'compact
          @item{去@hyperlink["https://racket-lang.org/download/"]{官方网站}下载、安装，这一步比较简单。}
          @item{回顾@secref{env-conf}，将@filepath{C:\Program Files\Racket}加到环境变量@envvar{Path}中。
                  碰巧@tamer-figure-ref{path}里第一条就是。}
          @item{关闭 PowerShell 再重新打开，安装我的软件包。
                 @itemlist[#:style 'compact
                           @commandline{raco pkg install -i --auto digimon graphics w3s mox}]}]

完毕。

@handbook-scenario{SDL2}

SDL2 是 Simple DirectMedia Layer 2.0，一个跨平台的 2D 游戏引擎，也是 Python 的游戏库也以它为基石。

在本系列课程中，SDL2 是 C++ 和 Python 代码库的底层接口，用于显示窗口并在里面播放动画或运行游戏，
不需要下载源码，需要用操作系统的软件包管理工具(不同于软件商店)安装二进制动态链接库。

@handbook-action{Windows 下使用 vcpkg 安装 SDL2}

vcpkg 是微软自己的软件包管理工具，用于安装与自己的编译器兼容的第三方软件库，其中就包括我们需要的 SDL2。

vcpkg 没有现成的可执行文件，而是直接提供源码用 git 来安装和升级。接下来的步骤需要在 PowerShell 里运行（每个“--”引导的行都是一条命令）：

@itemlist[#:style 'compact
          @item{下载 vcpkg 并安装在 @filepath{C:\opt\vcpkg} 目录里。
                 @itemlist[#:style 'compact
                           @commandline{git clone https://github.com/microsoft/vcpkg C:\opt\vcpkg}
                           @commandline{C:\opt\vcpkg\bootstrap-vcpkg.bat}]}

          @item{安装 64bit SDL2
                 @itemlist[#:style 'compact
                           @commandline{C:\opt\vcpkg\vcpkg install sdl2 sdl2_ttf sdl2_image sdl2_gfx sdl2_mixer sdl2_net --triplet=x64-windows}]}
          
          @item{用管理员身份运行(普通用户运行时可现场授权，无需另开终端)
                 @itemlist[#:style 'compact
                           @commandline{C:\opt\vcpkg\vcpkg integrate install}]}
          
          @item{回顾@secref{env-conf}，将@filepath{C:\opt\vcpkg}和@filepath{C:\opt\vcpkg\installed\x64-windows\bin}两项分别加到环境变量@envvar{Path}中。}]

自此，SDL2 安装完毕。

@handbook-scenario{Visual Studio}

Visual Studio 是微软自己的标准IDE(集成开发环境，包括编辑、编译、合作等一系列专业软件开发的工作流)。

对学生而言 Visual Studio 可能过于庞杂了，但在本系列课程中，安装它主要是为了跟它一起安装的 C++ 编译器。
去@hyperlink["https://visualstudio.microsoft.com/downloads/"]{官方网站}下载免费的 Community 版，然后对照
@tamer-figure-ref{vs}至少勾选@onscreen{Desktop Development with C++}一项。如果你想在 Visual Studio 里折腾
Python，可以再勾选@onscreen{Python Development}，然后在右侧的@onscreen{optional}栏里第一项手动勾选 Python 解释器。
@tamer-figure*["vs" "安装 Visual Studio（含 C++ 和 Python）"]{@image["stone/installation/vs.jpg" #:scale 0.36]}
安装过程比较漫长，耐心等待即可。之后继续配置@envvar{Path}环境变量，详细过程见 @secref{env-conf}，
将@filepath{C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build}加进去。
将来 Visual Studio 出新版本了不会影响这个2022版的，除非你自己把它删了。

@handbook-scenario{Visual Studio Code}

Visual Studio Code 也是微软推出的产品，但它主要职能是“编辑”代码，功能比 Visual Studio 少很多。
Visual Studio Code 是开源软件，别人可以一起贡献代码，因而安装了合适扩展就可以编辑所有语言的代码，
且字体、配色、操作风格一致，这一点 Visual Studio 是做不到的。
对学生而言 Visual Studio Code 足够轻便，也有利于他们分清楚“编辑”和“编译”两个过程。

它的安装更为简单，还是去同一个@hyperlink["https://visualstudio.microsoft.com/downloads/"]{页面}找到那个大大的
Visual Studio Code，下载安装一气呵成。

之后，当你打开一个明显是代码的文件时，它会提醒你安装相应语言的扩展，然后就可以愉快地编码了。要运行 C++ 有点麻烦，可能是因为微软的编译器
依赖一大堆环境变量，而这些变量又随时可能变化，普遍程序员的配置成本太高了。前面我们配置的那个环境变量并不能直接运行 C++ 编译器，
但里面有得到所有环境变量的程序。后面我们再细说运行的事。

@handbook-scenario{Python}

Python 是什么不需要我再强调了，地球人都知道，不过它也就“用的人多”这一个优点。

虽然安装 Visual Studio 会同时安装一个 Python，不过我还是建议自己额外安装，最起码版本比较新。
而且，现在在 PowerShell 里已经有 @exec{python.exe} 了，这是一个空壳，会引导去商店安装，不过我这商店一直无法联网，不折腾了。

去@hyperlink["https://www.python.org/downloads/windows/"]{官方网站}下载与你的体系结构相对应的
@onscreen{installer}，现在一般都是@tt{64-bit}的。注意，一定要是@onscreen{installer}，不能是@onscreen{embeddable package}。

@tamer-figure-here["python" "安装 Python"]{@image["stone/installation/python.png" #:scale figure-scale]}
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

那个 PyGame 是 Python 自己提供的建立在 PySDL2 基础之上的游戏引擎。
按理说，不装它也没有问题，但实际情况是不装它我们的程序也运行不起来。
大概是这几个软件包的依赖关系比较混乱，凑合着用把。

@handbook-scenario{Scratch}

目前 Scratch 无需特别配置，安装完开箱即用。
