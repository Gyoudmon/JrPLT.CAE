import sdl2
import sdl2.sdlgfx as gfx

import math
import ctypes

from ..igraphlet import *
from ..movable import *

from ...graphics.image import *
from ...graphics.colorspace import *

###################################################################################################
class IShapelet(IGraphlet, IMovable):
    def __init__(self, color = -1, border_color = -1):
        super(IShapelet, self).__init__()
        self.enable_resizing(True)
        self.__geometry = None
        self.__mixture = ColorMixture.Alpha
        self.__color = color
        self.__border_color = border_color
        self.__alpha = 0xFF
        
    def __del__(self):
        self._invalidate_geometry()

# public
    def draw(self, renderer, flx, fly, flWidth, flHeight):
        width = round(flWidth)
        height = round(flHeight)
        
        if not self.__geometry:
            self.__geometry = game_blank_image(renderer, width + 1, height + 1)

            if self.__geometry:
                origin = sdl2.SDL_GetRenderTarget(renderer)

                sdl2.SDL_SetRenderTarget(renderer, self.__geometry)
        
                if self.__color >= 0:
                    r, g, b = RGB_FromHexadecimal(self.__color)
                    self._fill_shape(renderer, width, height, r, g, b, self.__alpha)

                if self.__border_color >= 0:
                    r, g, b = RGB_FromHexadecimal(self.__border_color)
                    self._draw_shape(renderer, width, height, r, g, b, self.__alpha)

                sdl2.SDL_SetRenderTarget(renderer, origin)
                sdl2.SDL_SetTextureBlendMode(self.__geometry, color_mixture_to_blend_mode(self.__mixture))
            else:
                print("无法绘制几何图形：%s" % sdl2.SDL_GetError().decode('utf-8'))
        
        if self.__geometry:
            game_render_texture(renderer, self.__geometry, (flx, fly, flWidth, flHeight))

# public
    def set_color(self, color):
        if self.__color != color:
            self.__color = color
            self._invalidate_geometry()
            self.notify_updated()

    def get_color(self):
        return self.__color

    def set_border_color(self, color):
        if self.__border_color != color:
            self.__border_color = color
            self._invalidate_geometry()
            self.notify_updated()
    
    def get_border_color(self):
        return self.__border_color
    
    def set_alpha(self, a):
        if isinstance(a, float):
            if a >= 1.0:
                a = 0xFF
            elif a <= 0.0:
                a = 0
            else:
                a = round(a * 255.0)

        if self.__alpha != a:
            self.__alpha = a
            self._invalidate_geometry()
            self.notify_updated()

    def set_color_mixture(self, mixture):
        if self.__mixture != mixture:
            self.__mixture = mixture
            self._invalidate_geometry()
            self.notify_updated()

# protected
    def _draw_shape(self, renderer, width, height, r, g, b, a): pass
    def _fill_shape(self, renderer, width, height, r, g, b, a): pass

# protected
    def _invalidate_geometry(self):
        if self.__geometry:
            sdl2.SDL_DestroyTexture(self.__geometry)
            self.__geometry = None

###################################################################################################
class Linelet(IShapelet):
    def __init__(self, ex, ey, color):
        super(Linelet, self).__init__(color, -1)
        self.__epx = ex
        self.__epy = ey
        
    def get_extent(self, x, y):
        return abs(self.__epx), abs(self.__epy)

    def _on_resize(self, w, h, width, height):
        self.__epx *= w / width
        self.__epy *= h / height
        self._invalidate_geometry()

    def _fill_shape(self, renderer, width, height, r, g, b, a):
        x, y = 0, 0
        xn, yn = round(self.__epx), round(self.__epy)

        if xn < 0:
            x = x - xn
        
        if yn < 0:
            y = y - yn

        gfx.aalineRGBA(renderer, x, y, x + xn, y + yn, r, g, b, a)        

class HLinelet(Linelet):
    def __init__(self, width, color):
        super(HLinelet, self).__init__(width, 0.0, color)

class VLinelet(Linelet):
    def __init__(self, height, color):
        super(VLinelet, self).__init__(0.0, height, color)

###################################################################################################
class Rectanglet(IShapelet):
    def __init__(self, width, height, color, border_color = -1):
        super(Rectanglet, self).__init__(color, border_color)
        self.__width, self.__height = width, height

    def get_extent(self, x, y):
        return self.__width, self.__height

    def _on_resize(self, w, h, width, height):
        self.__width = w
        self.__height = h
        self._invalidate_geometry()
    
    def _draw_shape(self, renderer, width, height, r, g, b, a):
        gfx.rectangleRGBA(renderer, width, 0, 0, height, r, g, b, a)

    def _fill_shape(self, renderer, width, height, r, g, b, a):
        gfx.boxRGBA(renderer, width, 0, 0, height, r, g, b, a)

class Squarelet(Rectanglet):
    def __init__(self, edge_size, color, border_color=-1):
        super(Squarelet, self).__init__(edge_size, edge_size, color, border_color)

class RoundedRectanglet(IShapelet):
    def __init__(self, width, height, radius, color, border_color = -1):
        super(RoundedRectanglet, self).__init__(color, border_color)
        self.__width, self.__height = width, height
        self.__radius = radius

    def get_extent(self, x, y):
        return self.__width, self.__height

    def _on_resize(self, w, h, width, height):
        self.__width = w
        self.__height = h
        self._invalidate_geometry()
    
    def _draw_shape(self, renderer, width, height, r, g, b, a):
        rad = self.__radius

        if rad < 0.0:
            rad = -min(self.__width, self.__height) * rad

        gfx.roundedRectangleRGBA(renderer, 0, 0, width, height, round(rad), r, g, b, a)

    def _fill_shape(self, renderer, width, height, r, g, b, a):
        rad = self.__radius

        if rad < 0.0:
            rad = -min(self.__width, self.__height) * rad

        gfx.roundedBoxRGBA(renderer, 0, 0, width, height, round(rad), r, g, b, a)

class RoundedSquarelet(RoundedRectanglet):
    def __init__(self, edge_size, radius, color, border_color = -1):
        super(RoundedSquarelet, self).__init__(edge_size, edge_size, radius, color, border_color)

class Ellipselet(IShapelet):
    def __init__(self, aradius, bradius, color, border_color = -1):
        super(Ellipselet, self).__init__(color, border_color)
        self.__aradius, self.__bradius = aradius, bradius

    def get_extent(self, x, y):
        return self.__aradius * 2.0, self.__bradius * 2.0

    def _on_resize(self, w, h, width, height):
        self.__aradius = w * 0.5
        self.__bradius = h * 0.5
        self._invalidate_geometry()
    
    def _draw_shape(self, renderer, width, height, r, g, b, a):
        rx = round(self.__aradius) - 1
        ry = round(self.__bradius) - 1
        cx = rx + 1
        cy = ry + 1

        if rx == ry:
            gfx.aacircleRGBA(renderer, cx, cy, rx, r, g, b, a)
        else:
            gfx.aaellipseRGBA(renderer, cx, cy, rx, ry, r, g, b, a)

    def _fill_shape(self, renderer, width, height, r, g, b, a):
        rx = round(self.__aradius) - 1
        ry = round(self.__bradius) - 1
        cx = rx + 1
        cy = ry + 1

        if rx == ry:
            gfx.filledCircleRGBA(renderer, cx, cy, rx, r, g, b, a)
            gfx.aacircleRGBA(renderer, cx, cy, rx, r, g, b, a)
        else:
            gfx.filledEllipseRGBA(renderer, cx, cy, rx, ry, r, g, b, a)
            gfx.aaellipseRGBA(renderer, cx, cy, rx, ry, r, g, b, a)

class Circlet(Ellipselet):
    def __init__(self, radius, color, border_color = -1):
        super(Circlet, self).__init__(radius, radius, color, border_color)

###################################################################################################
class RegularPolygonlet(IShapelet):
    def __init__(self, n, radius, color, border_color = -1, rotation = 0.0):
        super(RegularPolygonlet, self).__init__(color, border_color)
        self.__PTArray = ctypes.c_int16 * (n + 1)
        self.__n = n
        self.__aradius, self.__bradius = radius, radius
        self.__rotation = rotation
        self.__lx, self.__rx, self.__ty, self.__by = 0.0, 0.0, 0.0, 0.0
        self.__xs = self.__PTArray()
        self.__ys = self.__PTArray()
        self.__initialize_vertice()

    def get_extent(self, x, y):
        return self.__rx - self.__lx + 1, self.__by - self.__ty + 1
    
    def _on_resize(self, w, h, width, height):
        self.__aradius = w / width
        self.__bradius = h / height
        self.__initialize_vertice()
        self._invalidate_geometry()

    def _draw_shape(self, renderer, width, height, r, g, b, a):
        gfx.aapolygonRGBA(renderer, self.__xs, self.__ys, self.__n, r, g, b, a)
    
    def _fill_shape(self, renderer, width, height, r, g, b, a):
        gfx.filledPolygonRGBA(renderer, self.__xs, self.__ys, self.__n, r, g, b, a)
        gfx.aapolygonRGBA(renderer, self.__xs, self.__ys, self.__n, r, g, b, a)
    
    def __initialize_vertice(self):
        start = math.radians(self.__rotation)
        delta = 2.0 * math.pi / float(self.__n)

        self.__lx = self.__aradius
        self.__ty = self.__bradius
        self.__rx = -self.__lx
        self.__by = -self.__ty

        for idx in range(0, self.__n):
            theta = start + delta * float(idx)
            self.__xs[idx] = this_x = round(self.__aradius * math.cos(theta))
            self.__ys[idx] = this_y = round(self.__bradius * math.sin(theta))

            if self.__rx < this_x:
                self.__rx = this_x
            elif self.__lx > this_x:
                self.__lx = this_x

            if self.__by < this_y:
                self.__by = this_y
            elif self.__ty > this_y:
                self.__ty = this_y

        for idx in range(0, self.__n):
            self.__xs[idx] -= self.__lx
            self.__ys[idx] -= self.__ty
