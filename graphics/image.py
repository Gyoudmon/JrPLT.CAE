import os
import ctypes as ffi

import sdl2
import sdl2.sdlimage as sdlimg

from .geometry import *
from .colorspace import *

###################################################################################################
pSurface = ffi.POINTER(sdl2.SDL_Surface)
pTexture = ffi.POINTER(sdl2.SDL_Texture)

###################################################################################################
def game_blank_image(renderer, width, height):
    img = sdl2.SDL_CreateTexture(renderer,
            sdl2.SDL_PIXELFORMAT_RGBA8888, sdl2.SDL_TEXTUREACCESS_TARGET,
            round(width), round(height))
    
    if img:
        origin = sdl2.SDL_GetRenderTarget(renderer)

        sdl2.SDL_SetTextureBlendMode(img, sdl2.SDL_BLENDMODE_BLEND)

        sdl2.SDL_SetRenderTarget(renderer, img)
        sdl2.SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0)
        sdl2.SDL_RenderClear(renderer)
        sdl2.SDL_SetRenderTarget(renderer, origin)
    
    return img

def game_formatted_surface(width, height, format):
    return sdl2.SDL_CreateRGBSurfaceWithFormat(0, round(width), round(height), 32, format)

def game_load_image(renderer, file):
    return sdlimg.IMG_LoadTexture(renderer, file.encode("utf-8"))

def game_unload_image(image):
    if isinstance(image, pSurface):
        sdl2.SDL_FreeSurface(image)
    else:
        sdl2.SDL_DestroyTexture(image)

def game_draw_image(renderer, image, pos, size = None, flip = None, double = 0.0):
    if (not size) or (image.w == size[0] and image.h == size[1]):
        game_render_texture(renderer, image, pos, flip, double)
    else:
        region = sdlimg.SDL_Rect(round(pos[0]), round(pos[1]), round(size[0]), round(size[1]))

        if size[0] <= 0: region.w = image.w
        if size[1] <= 0: region.h = image.h
        
        game_render_texture(renderer, image, region, flip, double)

###################################################################################################
def game_save_image(png, pname):
    okay = False

    if png:
        os.makedirs(os.path.dirname(pname), exist_ok = True)
        if sdlimg.IMG_SavePNG(png, pname.encode('utf-8')) == 0:
            okay = True

    return okay
