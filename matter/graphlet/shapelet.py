import sdl2
import sdl2.sdlgfx

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
        self.__color = color
        self.__border_color = border_color
        self.__alpha_color_key = 0xFFFFFF

    def __del__(self):
        if self.__geometry:
            sdl2.SDL_FreeSurface(self.__geometry)
            
# public
    def construct(self):
        width, height = self._get_shape_extent()
        geometry = game_blank_image(width, height, self.__alpha_color_key)

        if geometry:
            renderer = sdl2.SDL_CreateSoftwareRenderer(geometry)

            if renderer:
                RGB_SetRenderDrawColor(renderer, self.__alpha_color_key)
                sdl2.SDL_RenderClear(renderer)

                if self.__color >= 0:
                    r, g, b = RGB_FromHexadecimal(self.__color)
                    self._fill_shape(renderer, width, height, r, g, b, 0xFF)

                if self.__border_color >= 0:
                    r, g, b = RGB_FromHexadecimal(self.__border_color)
                    self._draw_shape(renderer, width, height, r, g, b, 0xFF)
                
                sdl2.SDL_RenderPresent(renderer)
                sdl2.SDL_DestroyRenderer(renderer)

        self._on_shape_changed(geometry)

    def get_extent(self, x, y):
        w, h = 0.0, 0.0

        if self.__geometry:
            g = self.__geometry.contents
            w, h = g.w, g.h
        
        return w, h

    def draw(self, renderer, x, y, Width, Height):
        if self.__geometry:
            ox, oy = self.get_shape_origin()
            game_render_surface(renderer, self.__geometry, (x - ox, y - oy))

# public
    def set_color(self, color):
        if self.__color != color:
            self.__color = color
            self.construct()

    def get_color(self):
        return self.__color

    def set_border_color(self, color):
        if self.__border_color != color:
            self.__border_color = color
            self.construct()
    
    def get_border_color(self):
        return self.__border_color

    def set_alpha_key_color(self, color):
        if self.__alpha_color_key != color:
            self.__alpha_color_key = color
            self.construct()

    def get_shape_origin(self):
        return 0.0, 0.0

# protected
    def _on_shape_changed(self, geometry):
        if self.__geometry:
            sdl2.SDL_FreeSurface(self.__geometry)

        self.__geometry = geometry
        self.notify_updated()

# protected
    def _get_shape_extent(self): return 0.0, 0.0
    def _draw_shape(self, renderer, width, height, r, g, b, a): pass
    def _fill_shape(self, renderer, width, height, r, g, b, a): pass

###################################################################################################
class Linelet(IShapelet):
    def __init__(self, ex, ey, color):
        super(Linelet, self).__init__(color, -1)
        self.__epx = ex
        self.__epy = ey
        
    def resize(self, width, height):
        if width > 0.0 and height > 0.0:
            w, h = self._get_shape_extent()

            if width != w or height != h:
                self.__epx *= width / w
                self.__epy *= height / h
                self.construct()

    def _get_shape_extent(self):
        w = max(abs(self.__epx), 1.0)
        h = max(abs(self.__epy), 1.0)

        return w, h

    def _fill_shape(self, renderer, width, height, r, g, b, a):
        x0, y0 = 0, 0
        xn, yn = round(self.__epx), round(self.__epy)

        if self.__epx < 0.0:
            x0 = -self.__epx
        
        if self.__epy < 0.0:
            y0 = -self.__epy

        sdl2.sdlgfx.aalineRGBA(renderer, x0, y0, x0 + xn, y0 + yn, r, g, b, a)        

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

    def resize(self, width, height):
        if width > 0.0 and height > 0.0:
            if self.__width != width or self.__height != height:
                self.__width = width
                self.__height = height
                self.construct()
    
    def _get_shape_extent(self):
        return self.__width, self.__height

    def _draw_shape(self, renderer, width, height, r, g, b, a):
        sdl2.sdlgfx.rectangleRGBA(renderer, 0, 0, width, height, r, g, b, a)

    def _fill_shape(self, renderer, width, height, r, g, b, a):
        sdl2.sdlgfx.boxRGBA(renderer, 0, 0, width, height, r, g, b, a)

class Squarelet(Rectanglet):
    def __init__(self, edge_size, color, border_color=-1):
        super(Squarelet, self).__init__(edge_size, edge_size, color, border_color)

class RoundedRectanglet(IShapelet):
    def __init__(self, width, height, radius, color, border_color = -1):
        super(RoundedRectanglet, self).__init__(color, border_color)
        self.__width, self.__height = width, height
        self.__radius = radius

    def resize(self, width, height):
        if width > 0.0 and height > 0.0:
            if self.__width != width or self.__height != height:
                self.__width = width
                self.__height = height
                self.construct()
    
    def _get_shape_extent(self):
        return self.__width, self.__height

    def _draw_shape(self, renderer, width, height, r, g, b, a):
        rad = self.__radius

        if rad < 0.0:
            rad = -min(self.__width, self.__height) * rad

        sdl2.sdlgfx.roundedRectangleRGBA(renderer, 0, 0, width, height, round(rad), r, g, b, a)

    def _fill_shape(self, renderer, width, height, r, g, b, a):
        rad = self.__radius

        if rad < 0.0:
            rad = -min(self.__width, self.__height) * rad

        sdl2.sdlgfx.roundedBoxRGBA(renderer, 0, 0, width, height, round(rad), r, g, b, a)

class RoundedSquarelet(RoundedRectanglet):
    def __init__(self, edge_size, radius, color, border_color = -1):
        super(RoundedSquarelet, self).__init__(edge_size, edge_size, radius, color, border_color)

class Ellipselet(IShapelet):
    def __init__(self, aradius, bradius, color, border_color = -1):
        super(Ellipselet, self).__init__(color, border_color)
        self.__aradius, self.__bradius = aradius, bradius

    def resize(self, width, height):
        if width > 0.0 and height > 0.0:
            ar = width * 0.5
            br = height * 0.5

            if self.__aradius != ar or self.__bradius != br:
                self.__aradius = ar
                self.__bradius = br
                self.construct()
    
    def _get_shape_extent(self):
        return self.__aradius * 2.0, self.__bradius * 2.0

    def _draw_shape(self, renderer, width, height, r, g, b, a):
        rx = round(self.__aradius) - 1
        ry = round(self.__bradius) - 1

        if rx == ry:
            sdl2.sdlgfx.aacircleRGBA(renderer, rx, ry, rx, r, g, b, a)
        else:
            sdl2.sdlgfx.aaellipseRGBA(renderer, rx, ry, rx, ry, r, g, b, a)

    def _fill_shape(self, renderer, width, height, r, g, b, a):
        rx = round(self.__aradius) - 1
        ry = round(self.__bradius) - 1

        if rx == ry:
            sdl2.sdlgfx.filledCircleRGBA(renderer, rx, ry, rx, r, g, b, a)
            sdl2.sdlgfx.aacircleRGBA(renderer, rx, ry, rx, r, g, b, a)
        else:
            sdl2.sdlgfx.filledEllipseRGBA(renderer, rx, ry, rx, ry, r, g, b, a)
            sdl2.sdlgfx.aaellipseRGBA(renderer, rx, ry, rx, ry, r, g, b, a)

class Circlet(Ellipselet):
    def __init__(self, radius, color, border_color = -1):
        super(Circlet, self).__init__(radius, radius, color, border_color)

###################################################################################################
class RegularPolygonlet(IShapelet):
    def __init__(self, n, radius, color, border_color = -1, rotation = 0.0):
        super(RegularPolygonlet, self).__init__(color, border_color)
        self.__n = n
        self.__aradius, self.__bradius = radius, radius
        self.__rotation = rotation
        self.__xs, self.__ys = None, None
        self.__initialize_vertice()

    def resize(self, width, height):
        if width > 0.0 and height > 0.0:
            ar = width * 0.5
            br = height * 0.5

            if self.__aradius != ar or self.__bradius != br:
                self.__aradius = ar
                self.__bradius = br
                self.__initialize_vertice()
                self.construct()


    def _get_shape_extent(self):
        return self.__aradius * 2.0, self.__bradius * 2.0
    
    def _draw_shape(self, renderer, width, height, r, g, b, a):
        sdl2.sdlgfx.aapolygonRGBA(renderer, self.__xs, self.__ys, self.__n, r, g, b, a)
    
    def _fill_shape(self, renderer, width, height, r, g, b, a):
        sdl2.sdlgfx.filledPolygonRGBA(renderer, self.__xs, self.__ys, self.__n, r, g, b, a)
        sdl2.sdlgfx.aapolygonRGBA(renderer, self.__xs, self.__ys, self.__n, r, g, b, a)
    
    def __initialize_vertice(self):
        start = math.radians(self.__rotation)
        delta = 2.0 * math.pi / float(self.__n)
        rx = self.__aradius - 0.5
        ry = self.__bradius - 0.5
        xs, ys = [], []

        for idx in range(0, self.__n):
            theta = start + delta * float(idx)
            xs.append(round(rx * math.cos(theta) + rx))
            ys.append(round(ry * math.sin(theta) + ry))

        self.__xs = (ctypes.c_int16 * self.__n)(*xs)
        self.__ys = (ctypes.c_int16 * self.__n)(*ys)
