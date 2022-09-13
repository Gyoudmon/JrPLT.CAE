#!/usr/bin/env python3

import sys # 系统相关参数和函数

import sdl2     # 原始 (C 风格) SDL2 函数
import sdl2.ext # Python 风格的 SDL2 函数

###############################################################################
def game_initialize:
    pass

###############################################################################
class Universe:
    def __init__(self, title = "Big Bang!", width = 1200, height = 800, fps = 60, fgc = 0xFFFFFFFF, bgc = 0x000000FF):
        ''' 构造函数，在创建对象时自动调用，以设置帧频、窗口标题、前背景色和混色模式 '''
        sdl2.SDL_Init(sdl2.SDL_INIT_VIDEO | sdl2.SDL_INIT_TIMER)
        
        self.fps = fps
        self.fgc = fgc
        self.bgc = bgc

        self.window = sdl2.SDL_CreateWindow(title.encode('utf-8'),
                sdl2.SDL_WINDOWPOS_CENTERED, sdl2.SDL_WINDOWPOS_CENTERED,
                width, height, sdl2.SDL_WINDOW_SHOWN)

        self.renderer = sdl2.SDL_CreateRenderer(self.window, -1, sdl2.SDL_RENDERER_ACCELERATED)
        self.texture = sdl2.SDL_CreateTexture(self.renderer, sdl2.SDL_PIXELFORMAT_RGBA8888,
                sdl2.SDL_TEXTUREACCESS_TARGET, width, height)

    def __del__(self):
        ''' 析构函数，在对象被销毁时自动调用 '''
        if self.texture:
            sdl2.SDL_DestroyTexture(self.texture)

        if self.renderer:
            sdl2.SDL_DestroyRenderer(self.renderer)

        if self.window:
            sdl2.SDL_DestroyWindow(self.window)

        sdl2.SDL_Quit()

    def construct(self, argv):
        pass

    def big_bang(self):
        quit_time = 0
        handled = False
        
        while (quit_time == 0):
            for e in sdl2.ext.get_events():
                if e.type == sdl2.SDL_QUIT:
                    quit_time = e.quit.timestamp
                    handled = False
        
            if handled:
                self.window.refresh()
                handled = False



###############################################################################
# __name__ 是一个特殊变量
# 可用于提示是否从这行代码启动应用程序
# 效果上相当于 C++ 程序的启动器，负责调用 main 函数
if __name__=="__main__":

    # 混沌初开，宇宙诞生
    universe = Universe("The Pong Game", 1200, 800)

    # 创建游戏世界
    universe.construct(sys.argv)

    # 宇宙大爆炸
    # 开启游戏主循环，直到玩家关闭游戏
    universe.big_bang()

    # Python 心满意足地退出
    # 顺便销毁游戏宇宙，回归虚无
    sys.exit(0)


