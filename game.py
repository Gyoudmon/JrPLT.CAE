import sys              # 系统相关参数和函数
import os               # 操作系统相关函数
import ctypes           # 外语接口
import atexit           # 用于管理程序退出时执行的函数

import sdl2             # 原始 (C 风格) SDL2 函数
import sdl2.sdlttf      # 原始 (C 风格) SDL2_TTF 函数
import sdl2.sdlimage    # 原始 (C 风格) SDL2_Image 函数

import sdl2.ext         # Python 风格的 SDL2 函数

from .font import *
from .colorspace import *

###############################################################################
def game_initialize(flags, fontsize = 16):
    if game_font.DEFAULT is None:
        _Call_With_Safe_Exit(sdl2.SDL_Init(flags), "SDL 初始化失败：", sdl2.SDL_Quit)
        _Call_With_Safe_Exit(sdl2.sdlttf.TTF_Init(), "TTF 初始化失败：", sdl2.sdlttf.TTF_Quit, sdl2.sdlttf.TTF_GetError)

        if sys.platform == "darwin":
            sdl2.sdlimage.IMG_Init(sdl2.sdlimage.IMG_INIT_JPG | sdl2.sdlimage.IMG_INIT_PNG)
        else:
            sdl2.sdlimage.IMG_Init(sdl2.sdlimage.IMG_INIT_PNG)
    
        maybe_err = sdl2.sdlimage.IMG_GetError()
        if len(maybe_err):
            print("IMG 初始化失败：" + maybe_err)
            os._exit(1)

        game_fonts_initialize(fontsize)

        atexit.register(sdl2.sdlimage.IMG_Quit)
        # atexit.register(game_fonts_destroy) # leave it to TTF_Quit

def game_world_create(title, width, height):
    cpos = sdl2.SDL_WINDOWPOS_CENTERED

    w = sdl2.SDL_CreateWindow(title.encode("utf-8"), cpos, cpos, width, height, sdl2.SDL_WINDOW_SHOWN)
    _Check_Variable_Validity(w, None, "SDL 窗体创建失败：")

    r = sdl2.SDL_CreateRenderer(w, -1, sdl2.SDL_RENDERER_ACCELERATED)
    _Check_Variable_Validity(r, None, "SDL 渲染器创建失败：")

    t = sdl2.SDL_CreateTexture(r, sdl2.SDL_PIXELFORMAT_RGBA8888, sdl2.SDL_TEXTUREACCESS_TARGET, width, height)
    _Check_Variable_Validity(t, None, "SDL 纹理创建失败：")

    return w, r, t

def game_world_reset(renderer, fgc, bgc, texture = None):
    fcr, fcg, fcb = RGB_FromHexadecimal(fgc)
    bcr, bcg, bcb = RGB_FromHexadecimal(bgc)

    if not (texture is None):
        sdl2.SDL_SetRenderTarget(renderer, texture)

    sdl2.SDL_SetRenderDrawColor(renderer, bcr, bcg, bcb, 255)
    sdl2.SDL_RenderClear(renderer)
    sdl2.SDL_SetRenderDrawColor(renderer, fcr, fcg, fcb, 255)

def game_world_refresh(renderer, texture):
    sdl2.SDL_SetRenderTarget(renderer, None)
    sdl2.SDL_RenderCopy(renderer, texture, None, None)
    sdl2.SDL_RenderPresent(renderer)
    sdl2.SDL_SetRenderTarget(renderer, texture)

###############################################################################
class Universe(object):
    def __init__(self, title, width = 1200, height = 800, fps = 60, fgc = 0xFFFFFF, bgc = 0x000000):
        """ 构造函数，在创建游戏世界时自动调用，以设置帧频、窗口标题、前背景色和混色模式 """
        game_initialize(sdl2.SDL_INIT_VIDEO | sdl2.SDL_INIT_TIMER)
        
        # Please search "Python sequence unpacking"(序列解包)
        self.window, self.renderer, self.texture = game_world_create(title, width, height)
        self.fps, self.fgc, self.bgc, self.timer = fps, fgc, bgc, 0

        game_world_reset(self.renderer, self.fgc, self.bgc, self.texture)

        # Python doesn't need two-step initialization as C++ does
        self.construct(sys.argv)

    def __del__(self):
        """ 析构函数，在对象被销毁时自动调用，默认销毁游戏宇宙 """
        if self.texture:
            sdl2.SDL_DestroyTexture(self.texture)

        if self.renderer:
            sdl2.SDL_DestroyRenderer(self.renderer)

        if self.window:
            sdl2.SDL_DestroyWindow(self.window)

    def construct(self, argv):
        """ 给游戏世界一个解析命令行参数的机会，默认什么都不做 """
        pass

    def update(self, interval, count, uptime):
        """ 更新游戏世界，定时器到期时自动调用，默认什么都不做 """
        pass

    def draw(self, renderer, x, y, width, height):
        """ 绘制游戏世界，在合适的时候自动调用，默认什么都不做 """
        pass

    def can_exit(self):
        """ 告诉游戏主循环，是否游戏已经结束可以退出了，默认永久运行 """
        return False

    def big_bang(self):
        """ 宇宙大爆炸，开启游戏主循环，返回游戏运行时间 """
        quit_time = 0
        handled = False

        width, height = self.get_window_size()
        sdl2.SDL_SetRenderTarget(self.renderer, self.texture)
        self.draw(self.renderer, 0, 0, width, height)
        game_world_refresh(self.renderer, self.texture)
        
        while (quit_time == 0) and not self.can_exit():
            for e in sdl2.ext.get_events():
                if e.type == sdl2.SDL_QUIT:
                    quit_time = e.quit.timestamp
                    handled = False
        
            if handled:
                game_world_refresh(self.renderer, self.texture)
                handled = False

    def set_window_title(self, title):
        sdl2.SDL_SetWindowTitle(self.window, title.encode("utf-8"))

    def set_window_size(self, width, height):
        sdl2.SDL_SetWindowSize(self.window, width, height)

    def get_window_size(self):
        w = ctypes.c_int()
        h = ctypes.c_int()
        
        sdl2.SDL_GetWindowSize(self.window, ctypes.byref(w), ctypes.byref(h))

        return w, h

    def get_foreground_color(self):
        return self.fgc

    def get_background_color(self):
        return self.bgc

# protected
    def _on_frame(self, interval, count, uptime):
        game_world_reset(self.renderer, self.fgc, self.bgc)

# private
    def __on_elapse(self, interval, count, uptime):
        """ 响应定时器事件，刷新游戏世界 """
        self.uptime(interval, count, uptime)
        self._on_frame(interval, count, uptime)

###############################################################################
class Pasteboard(Universe):
    def __init__(self, title, width = 1200, height = 800, fgc = 0x000000, bgc = 0xFFFFFF):
        super(Pasteboard, self).__init__(title, width, height, 0, fgc, bgc)

###############################################################################
def _Call_With_Error_Message(init, message, GetError):
    if init != 0:
        print(message + GetError().decode("utf-8"))
        os._exit(1)
    
def _Call_With_Safe_Exit(init, message, fquit, GetError = sdl2.SDL_GetError):
    _Call_With_Error_Message(init, message, GetError)
    atexit.register(fquit)

def _Check_Variable_Validity(init, failure, message, GetError = sdl2.SDL_GetError):
    if (init == failure):
        print(message + GetError().decode("utf-8"))
        os._exit(1)
        
