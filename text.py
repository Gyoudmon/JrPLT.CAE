import sdl2                 # 原始 SDL2 函数
import sdl2.pixels as sdlp  # 原始 SDL2 像素结构体
import ctypes as ffi        # 外语接口
import sys

from .font import *
from .colorspace import *
from .geometry import *

###################################################################################################
TextRenderMode_Solid = 1
TextRenderMode_Shaded = 2
TextRenderMode_LCD = 3
TextRenderMode_Blender = 4

###################################################################################################
def game_text_size(font, width, height, text):
    if not font: 
        font = game_font.DEFAULT

    return _unsafe_utf8_size(font, text)

###################################################################################################
def game_text_surface(rtext, font, mode, fgc, bgc, wrap = 0):
    text = rtext.encode('utf-8')

    if not font:
        font = game_font.DEFAULT
    
    if sys.platform == 'win32':
        if mode == TextRenderMode_Solid:
            surface = sdl2.sdlttf.TTF_RenderUTF8_Solid(font, text, fgc)
        elif mode == TextRenderMode_Blender:
            surface = sdl2.sdlttf.TTF_RenderUTF8_Blended(font, text, fgc)
        else:
            surface = sdl2.sdlttf.TTF_RenderUTF8_Shaded(font, text, fgc, bgc)
    else:
        if wrap >= 0: # will wrap by newline for 0
            if mode == TextRenderMode_Solid:
                surface = sdl2.sdlttf.TTF_RenderUTF8_Solid_Wrapped(font, text, fgc, wrap)
            elif mode == TextRenderMode_Blender:
                surface = sdl2.sdlttf.TTF_RenderUTF8_Blended_Wrapped(font, text, fgc, wrap)
            elif mode == TextRenderMode_LCD:
                surface = sdl2.sdlttf.TTF_RenderUTF8_LCD_Wrapped(font, text, fgc, bgc, wrap)
            else:
                surface = sdl2.sdlttf.TTF_RenderUTF8_Shaded_Wrapped(font, text, fgc, bgc, wrap)
        else:
            if mode == TextRenderMode_Solid:
                surface = sdl2.sdlttf.TTF_RenderUTF8_Solid(font, text, fgc)
            elif mode == TextRenderMode_Blender:
                surface = sdl2.sdlttf.TTF_RenderUTF8_Blended(font, text, fgc)
            elif mode == TextRenderMode_LCD:
                surface = sdl2.sdlttf.TTF_RenderUTF8_LCD(font, text, fgc, bgc)
            else:
                surface = sdl2.sdlttf.TTF_RenderUTF8_Shaded(font, text, fgc, bgc)

    if not surface:
        print("无法渲染文本: " + sdl2.sdlttf.TTF_GetError().decode('utf-8'))

    return surface

###################################################################################################
def game_draw_solid_text(font, renderer, rgb, x, y, text, wrap = 0):
    message = _solid_text_surface(font, rgb, text, wrap)
    _safe_render_text_surface(renderer, message, x, y)

def game_draw_shaded_text(font, renderer, fgc, bgc, x, y, text, wrap = 0):
    message = _shaded_text_surface(font, fgc, bgc, text, wrap)
    _safe_render_text_surface(renderer, message, x, y)

def game_draw_lcd_text(font, renderer, fgc, bgc, x, y, text, wrap = 0):
    message = _lcd_text_surface(font, fgc, bgc, text, wrap)
    _safe_render_text_surface(renderer, message, x, y)

def game_draw_blended_text(font, renderer, rgb, x, y, text, wrap = 0):
    message = _blended_text_surface(font, rgb, text, wrap)
    _safe_render_text_surface(renderer, message, x, y)

###################################################################################################
def _unsafe_utf8_size(font, text):
    w = c_int(0)
    h = c_int(0)


    if TTF_SizeUTF8(font, text.encode('utf-8'), byref(w), byref(h)) == -1:
        print("无法计算文本尺寸: " + sdl2.sdlttf.TTF_GetError().decode('utf-8'))

    return (w.value, h.value)

def _hex_rgb_to_color(rgb):
    r, g, b = RGB_FromHexadecimal(rgb)

    return sdlp.SDL_Color(r, g, b, 255)

def _safe_render_text_surface(target, message, x, y):
    if not message == None:
        game_render_surface_at(target, message, x, y)
        sdl2.SDL_FreeSurface(message)

def _solid_text_surface(font, rgb, text, wrap):
    text_color = _hex_rgb_to_color(rgb)

    return game_text_surface(text, font, TextRenderMode_Solid, text_color, text_color, wrap)

def _shaded_text_surface(font, fgc, bgc, text, wrap):
    text_color = _hex_rgb_to_color(fgc)
    background_color = _hex_rgb_to_color(bgc)

    return game_text_surface(text, font, TextRenderMode_Shaded, text_color, background_color, wrap)

def _lcd_text_surface(font, fgc, bgc, text, wrap):
    text_color = _hex_rgb_to_color(fgc)
    background_color = _hex_rgb_to_color(bgc)

    return game_text_surface(text, font, TextRenderMode_LCD, text_color, background_color, wrap)

def _blended_text_surface(font, rgb, text, wrap):
    text_color = _hex_rgb_to_color(rgb)

    return game_text_surface(text, font, TextRenderMode_Blender, text_color, text_color, wrap)

