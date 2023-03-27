import pygame

import os
import sys

###################################################################################################
class game_font:
    DEFAULT: pygame.font.Font = None
    sans_serif: pygame.font.Font = None
    serif: pygame.font.Font = None
    monospace: pygame.font.Font = None
    math: pygame.font.Font = None
    unicode: pygame.font.Font = None

###################################################################################################
_default_fontsize = 16

_system_fonts = {}
_system_fontdirs = [
    "/System/Library/Fonts",
    "/Library/Fonts",
    "C:\\Windows\\Fonts",
    "/usr/share/fonts"
]

###################################################################################################
def game_fonts_initialize(fontsize = _default_fontsize):
    for rootdir in _system_fontdirs:
        if os.path.isdir(rootdir):
            for parent, _subdirs, fontfiles in os.walk(rootdir):
                for fontfile in fontfiles:
                    _system_fonts[fontfile] = (parent + os.sep + fontfile).encode('utf-8')

    if sys.platform == 'darwin':
        game_font.sans_serif = game_create_font("LucidaGrande.ttc", fontsize)
        game_font.serif = game_create_font("Times.ttc", fontsize)
        game_font.monospace = game_create_font("Courier.ttc", fontsize)
        game_font.math = game_create_font("Bodoni 72.ttc", fontsize)
        game_font.unicode = game_create_font("PingFang.ttc", fontsize)
    elif sys.platform == 'win32': # HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Fonts
        game_font.sans_serif = game_create_font("msyh.ttc", fontsize) # Microsoft YaHei
        game_font.serif = game_create_font("times.ttf", fontsize) # Times New Roman
        game_font.monospace = game_create_font("cour.ttf", fontsize) # Courier New
        game_font.math = game_create_font("BOD_R.TTF", fontsize) # Bodoni MT
        game_font.unicode = game_create_font("msyh.ttc", fontsize)
    else: ## the following fonts have not been tested ##
        game_font.sans_serif = game_create_font("Nimbus Sans.ttc", fontsize)
        game_font.serif = game_create_font("DejaVu Serif.ttc", fontsize)
        game_font.monospace = game_create_font("Monospace.ttf", fontsize)
        game_font.math = game_create_font("URW Bookman.ttf", fontsize)
        game_font.unicode = game_create_font("Arial Unicode.ttf", fontsize)

    game_font.DEFAULT = game_font.sans_serif;

def game_fonts_destroy():
    if game_font.DEFAULT != None:
        game_font.DEFAULT = None

    if game_font.sans_serif != None:
        game_font.sans_serif = None

    if game_font.serif != None:
        game_font.serif = None

    if game_font.monospace != None:
        game_font.monospace = None

    if game_font.math != None:
        game_font.math = None

    if game_font.unicode != None:
        game_font.unicode = None

###################################################################################################
def game_create_font(face, fontsize = _default_fontsize):
    font = None

    if face in _system_fonts:
        font = pygame.font.FontType(_system_fonts[face], fontsize)
    else:
        font = pygame.font.FontType(face, fontsize)

    return font

def game_font_destroy(font, usr_only = True):
    if font:
        if not usr_only: del font
        elif font == game_font.DEFAULT: pass
        elif font == game_font.sans_serif: pass
        elif font == game_font.serif: pass
        elif font == game_font.monospace: pass
        elif font == game_font.math: pass
        elif font == game_font.unicode: pass
        else: del font
