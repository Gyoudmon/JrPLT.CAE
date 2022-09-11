#!/usr/bin/env python3

import os  # 操作系统相关函数
import sys # 系统相关参数和函数

import sdl2     # 原始 SDL2 函数
import sdl2.ext # Python 风格的 SDL2 函数

###############################################################################
#class Universe:
#    def __init__():


###############################################################################
def main(argc, argv):
    sdl2.ext.init()
    window = sdl2.ext.Window("The Pong Game", size=(800, 600))
    window.show()
    running = True
    while running:
        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
        window.refresh()

    return 0



# __name__ 是一个特殊变量
# 可用于提示是否从这行代码启动应用程序
# 效果上相当于 C++ 程序的启动器，负责调用 main 函数
if __name__=="__main__":
    os._exit(main(len(sys.argv), sys.argv))

