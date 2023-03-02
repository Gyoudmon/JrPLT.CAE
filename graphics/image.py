import os
import pygame

from .geometry import *
from .colorspace import *

###################################################################################################
def game_blank_image(width, height, alpha_color_key = 0xFFFFFF):
    surface = pygame.Surface((round(width), round(height)), 0, 32)
    r, g, b = RGB_FromHexadecimal(alpha_color_key)
    surface.set_colorkey(pygame.Color(r, g, b))
    
    return surface

def game_load_image(file):
    return pygame.image.load(file)

def game_unload_image(image):
    del image

def game_draw_image(renderer, image, x, y):
    game_render_surface(renderer, image, (x, y))
    
def game_draw_image(renderer, file, x, y):
    image = game_load_image(file)

    if not image:
        game_draw_image(renderer, image, x, y)
        game_unload_image(image)

def game_draw_image(renderer, image, x, y, width, height):
    if image.w == width and image.h == height:
        game_render_surface(renderer, image, (x, y))
    else:
        region = pygame.rect(round(x), round(y), round(width), round(height))

        if  width <= 0: region.w = image.w
        if height <= 0: region.h = image.h
        
        game_render_surface(renderer, image, region)

def game_draw_image(renderer, file, x, y, width, height):
    image = game_load_image(file)

    if not image:
        game_draw_image(renderer, image, x, y, width, height)
        game_unload_image(image)

###################################################################################################
def game_save_image(png, pname):
    okay = False

    if png:
        os.makedirs(os.path.dirname(pname), exist_ok = True)
        pygame.image.save(png, pname)
        okay = True

    return okay
