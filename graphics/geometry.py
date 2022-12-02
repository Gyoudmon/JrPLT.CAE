import sdl2                             # 原始 SDL2 函数
import sdl2.rect as sdlr                # 原始 SDL2 矩形结构体
import ctypes as ffi                    # 外语接口函数
import math                             # 基础数学函数

from .colorspace import *               # 色彩空间相关函数, 前面那个点指代相对本文件的路径
from ..physics.mathematics import *     # 图形学、线性代数相关函数

###############################################################################
def game_draw_point(renderer, x, y, cs, alpha = 0xFF):
    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)

    sdl2.SDL_RenderDrawPoint(renderer, x, y)

def game_draw_line(renderer, x1, y1, x2, y2, cs, alpha = 0xFF):
    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)

    sdl2.SDL_RenderDrawLine(renderer, x1, y1, x2, y2)

def game_draw_rect(renderer, x, y, width, height, cs, alpha = 0xFF):
    box = sdlr.SDL_Rect(x, y, width, height)

    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)

    sdl2.SDL_RenderDrawRect(renderer, ffi.byref(box))

def game_fill_rect(renderer, x, y, width, height, cs, alpha = 0xFF):
    box = sdlr.SDL_Rect(round(x), round(y), round(width), round(height))

    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)

    sdl2.SDL_RenderFillRect(renderer, ffi.byref(box))

def game_draw_square(renderer, cx, cy, apothem, cs, alpha = 0xFF):
    game_draw_rect(renderer, cx - apothem, cy - apothem, apothem * 2, apothem * 2, cs, alpha)

def game_fill_square(renderer, cx, cy, apothem, cs, alpha = 0xFF):
    game_fill_rect(renderer, cx - apothem, cy - apothem, apothem * 2, apothem * 2, cs, alpha)

def game_draw_circle(renderer, cx, cy, radius, cs, alpha = 0xFF):
    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)

    _draw_circle(renderer, cx, cy, radius)

def game_fill_circle(renderer, cx, cy, radius, cs, alpha = 0xFF):
    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)

    _fill_circle(renderer, cx, cy, radius)

def game_draw_ellipse(renderer, cx, cy, aradius, bradius, cs, alpha = 0xFF):
    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)

    _draw_ellipse(renderer, cx, cy, aradius, bradius)

def game_fill_ellipse(renderer, cx, cy, aradius, bradius, cs, alpha = 0xFF):
    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)

    _fill_ellipse(renderer, cx, cy, aradius, bradius)

def game_draw_regular_polygon(renderer, n, cx, cy, radius, rotation, cs, alpha = 0xFF):
    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)    
        
    _draw_regular_polygon(renderer, n, cx, cy, radius, rotation)

def game_fill_regular_polygon(renderer, n, cx, cy, radius, rotation, cs, alpha = 0xFF):
    if isinstance(cs, int):
        RGB_SetRenderDrawColor(renderer, cs, alpha)
    else:
        HSV_SetRenderDrawColor(renderer, cs[0], cs[1], cs[2], alpha)    
        
    _fill_regular_polygon(renderer, n, cx, cy, radius, rotation)

###############################################################################
def game_render_surface(target, psurface, region):
    texture = sdl2.SDL_CreateTextureFromSurface(target, psurface)

    if not isinstance(region, sdlr.SDL_Rect):
        surface = psurface.contents # `contents` creates new instance every time
        region = sdlr.SDL_Rect(round(region[0]), round(region[1]), surface.w, surface.h)

    if texture:
        sdl2.SDL_RenderCopy(target, texture, None, region)
        sdl2.SDL_DestroyTexture(texture)

###############################################################################
def _draw_circle(renderer, cx, cy, radius):
    err = 2 - 2 * radius
    x = -radius
    y = 0
    
    while True:
        sdl2.SDL_RenderDrawPoint(renderer, cx + x, cy - y)
        sdl2.SDL_RenderDrawPoint(renderer, cx - x, cy + y)
        sdl2.SDL_RenderDrawPoint(renderer, cx + y, cy + x)
        sdl2.SDL_RenderDrawPoint(renderer, cx - y, cy - x)

        radius = err
        if radius <= y:
            y += 1
            err += y * 2 + 1

        if (radius > x) or (err > y):
            x += 1
            err += x * 2

        if x >= 0: break

def _fill_circle(renderer, cx, cy, radius):
    err = 2 - 2 * radius
    x = -radius
    y = 0
    
    while True:
        sdl2.SDL_RenderDrawLine(renderer, cx + x, cy + y, cx - x, cy + y)  # Q I, Q II
        sdl2.SDL_RenderDrawLine(renderer, cx + x, cy,     cx + x, cy - y)  # Q III
        sdl2.SDL_RenderDrawLine(renderer, cx - x, cy - y, cx,     cy - y)  # Q I

        radius = err
        if radius <= y:
            y += 1
            err += y * 2 + 1

        if (radius > x) or (err > y):
            x += 1
            err += x * 2 + 1
    
        if x >= 0: break

def _draw_ellipse(renderer, cx, cy, ar, br):
    # II. quadrant from bottom left to top right */
    x = -ar
    y = 0
    a2 = ar * ar
    b2 = br * br
    e2 = br
    dx = (1 + 2 * x) * e2 * e2
    dy = x * x
    err = dx + dy

    while True:
        sdl2.SDL_RenderDrawPoint(renderer, cx - x, cy + y)
        sdl2.SDL_RenderDrawPoint(renderer, cx + x, cy + y)
        sdl2.SDL_RenderDrawPoint(renderer, cx + x, cy - y)
        sdl2.SDL_RenderDrawPoint(renderer, cx - x, cy - y)

        e2 = 2 * err
        if e2 >= dx: # x step
            x += 1
            dx += 2 * b2
            err += dx
        if e2 <= dy: # y step
            y += 1
            dy += 2 * a2
            err += dy
    
        if x > 0: break

    # to early stop for flat ellipses with a = 1, finish tip of ellipse
    y += 1
    while y < br:
        sdl2.SDL_RenderDrawPoint(renderer, cx, cy + y)
        sdl2.SDL_RenderDrawPoint(renderer, cx, cy - y)
        y += 1

def _fill_ellipse(renderer, cx, cy, ar, br):
    # II. quadrant from bottom left to top right */
    x = -ar
    y = 0
    a2 = ar * ar
    b2 = br * br
    e2 = br
    dx = (1 + 2 * x) * e2 * e2
    dy = x * x
    err = dx + dy

    while True:
        sdl2.SDL_RenderDrawLine(renderer, cx + x, cy + y, cx - x, cy + y) # Q I, Q II
        sdl2.SDL_RenderDrawLine(renderer, cx + x, cy,     cx + x, cy - y) # Q III
        sdl2.SDL_RenderDrawLine(renderer, cx - x, cy - y, cx,     cy - y) # Q I

        e2 = 2 * err
        if e2 >= dx: # x step
            x += 1
            dx += 2 * b2
            err += dx
        if e2 <= dy: # y step
            y += 1
            dy += 2 * a2
            err += dy
    
        if x > 0: break

    # to early stop for flat ellipses with a = 1, finish tip of ellipse
    y += 1
    while y < br:
        sdl2.SDL_RenderDrawPoint(renderer, cx, cy + y, cx, cy - y)
        y += 1

def _draw_regular_polygon(renderer, n, cx, cy, r, rotation):
    # for inscribed regular polygon, the radius should be `Rcos(pi/n)`
    start = math.radians(rotation)
    delta = 2.0 * math.pi / float(n)

    x0 = px = r * math.cos(start) + cx
    y0 = py = r * math.sin(start) + cy

    for idx in range(1, n):
        theta = start + delta * float(idx)
        sx = r * math.cos(theta) + cx
        sy = r * math.sin(theta) + cy

        sdl2.SDL_RenderDrawLineF(renderer, px, py, sx, sy)
        px = sx
        py = sy

    if px != x0:
        sdl2.SDL_RenderDrawLineF(renderer, px, py, x0, y0)
    else:
        sdl2.SDL_RenderDrawPointF(renderer, cx, cy)

def _fill_regular_polygon(renderer, n, cx, cy, r, rotation):
    # for inscribed regular polygon, the radius should be `Rcos(pi/n)`
    start = math.radians(rotation)
    delta = 2.0 * math.pi / float(n)
    pts = []
    xmin = cx - r
    xmax = cx + r
    ymin = cy + r
    ymax = cy - r

    for idx in range(0, n):
        theta = start + delta * float(idx)
        sx = r * math.cos(theta) + cx
        sy = r * math.sin(theta) + cy

        pts.append(sdl2.SDL_FRect(sx, sy))

        if sy < ymin: ymin = sy
        if sy > ymax: ymax = sy
    
    pts.append(pts[0])

    y = ymin
    while y < ymax + 1.0:
        px = [0.0, 0.0]
        pcount = 0

        for i in range(0, n // 2 + 1):
            spt = pts[i]
            ept = pts[i + 1]

            px[pcount], py, t, _ = lines_intersection(spt.x, spt.y, ept.x, ept.y, xmin, y, xmax, y)
            if not math.isnan(t):
                if t >= 0.0 and t <= 1.0:
                    pcount += 1
            elif pcount == 0:
                px[0] = spt.x
                px[1] = ept.x
                pcount = 2
            
            if pcount == 2: break
 
            spt = pts[n - i]
            ept = pts[n - i - 1]
            
            px[pcount], py, t, _ = lines_intersection(spt.x, spt.y, ept.x, ept.y, xmin, y, xmax, y)
            if not math.isnan(t):
                if t >= 0.0 and t <= 1.0:
                    pcount += 1
            elif pcount == 0:
                px[0] = spt.x
                px[1] = ept.x
                pcount = 2

            if pcount == 2: break

        if pcount == 2:
            sdl2.SDL_RenderDrawLineF(renderer, px[0], y, px[1], y)
        elif n == 2:
            sdl2.SDL_RenderDrawPointF(renderer, px[0], py)
        elif n <= 1:
            sdl2.SDL_RenderDrawPointF(renderer, cx, cy)

        y += 1.0


###############################################################################
BISQUE = 0xffe4c4
MOCCASIN = 0xffe4b5
FUCHSIA = 0xff00ff
SKYBLUE = 0x87ceeb
SANDYBROWN = 0xf4a460
DARKBLUE = 0x8b
DIMGRAY = 0x696969
POWDERBLUE = 0xb0e0e6
LIGHTGREEN = 0x90ee90
RED = 0xff0000
ALICEBLUE = 0xf0f8ff
MEDIUMORCHID = 0xba55d3
DARKORANGE = 0xff8c00
NAVY = 0x80
GAINSBORO = 0xdcdcdc
CORNSILK = 0xfff8dc
GREEN = 0x8000
LIGHTSLATEGRAY = 0x778899
WHEAT = 0xf5deb3
OLIVE = 0x808000
IVORY = 0xfffff0
MISTYROSE = 0xffe4e1
LIGHTSEAGREEN = 0x20b2aa
GREENYELLOW = 0xadff2f
INDIANRED = 0xcd5c5c
DODGERBLUE = 0x1e90ff
YELLOWGREEN = 0x9acd32
ORANGE = 0xffa500
GREY = 0x808080
GOLD = 0xffd700
PALEVIOLETRED = 0xdb7093
VIOLET = 0xee82ee
CHOCOLATE = 0xd2691e
SLATEGREY = 0x708090
PALEGOLDENROD = 0xeee8aa
SEASHELL = 0xfff5ee
LAWNGREEN = 0x7cfc00
FIREBRICK = 0xb22222
PAPAYAWHIP = 0xffefd5
DARKRED = 0x8b0000
DARKVIOLET = 0x9400d3
DARKKHAKI = 0xbdb76b
ORCHID = 0xda70d6
DARKGRAY = 0xa9a9a9
DARKGREEN = 0x6400
SLATEBLUE = 0x6a5acd
TOMATO = 0xff6347
DARKSALMON = 0xe9967a
SPRINGGREEN = 0xff7f
SIENNA = 0xa0522d
DARKORCHID = 0x9932cc
TEAL = 0x8080
DEEPSKYBLUE = 0xbfff
CYAN = 0xffff
SLATEGRAY = 0x708090
DARKCYAN = 0x8b8b
MAROON = 0x800000
WHITESMOKE = 0xf5f5f5
AZURE = 0xf0ffff
WHITE = 0xffffff
DARKGREY = 0xa9a9a9
PLUM = 0xdda0dd
SADDLEBROWN = 0x8b4513
LIGHTSLATEGREY = 0x778899
LIGHTCORAL = 0xf08080
SALMON = 0xfa8072
KHAKI = 0xf0e68c
YELLOW = 0xffff00
PURPLE = 0x800080
MEDIUMVIOLETRED = 0xc71585
REBECCAPURPLE = 0x663399
PERU = 0xcd853f
TAN = 0xd2b48c
BEIGE = 0xf5f5dc
SNOW = 0xfffafa
THISTLE = 0xd8bfd8
LIGHTSTEELBLUE = 0xb0c4de
PEACHPUFF = 0xffdab9
LIGHTCYAN = 0xe0ffff
AQUA = 0xffff
DARKMAGENTA = 0x8b008b
LIGHTSKYBLUE = 0x87cefa
LIGHTGREY = 0xd3d3d3
DARKTURQUOISE = 0xced1
DARKSLATEBLUE = 0x483d8b
DARKGOLDENROD = 0xb8860b
LIGHTPINK = 0xffb6c1
MEDIUMPURPLE = 0x9370db
ANTIQUEWHITE = 0xfaebd7
STEELBLUE = 0x4682b4
BURLYWOOD = 0xdeb887
BLANCHEDALMOND = 0xffebcd
DARKSEAGREEN = 0x8fbc8f
DARKSLATEGREY = 0x2f4f4f
LIMEGREEN = 0x32cd32
MEDIUMTURQUOISE = 0x48d1cc
HONEYDEW = 0xf0fff0
MIDNIGHTBLUE = 0x191970
CHARTREUSE = 0x7fff00
DARKOLIVEGREEN = 0x556b2f
MAGENTA = 0xff00ff
LIME = 0xff00
ORANGERED = 0xff4500
ROSYBROWN = 0xbc8f8f
FORESTGREEN = 0x228b22
OLIVEDRAB = 0x6b8e23
GHOSTWHITE = 0xf8f8ff
PALETURQUOISE = 0xafeeee
LIGHTGOLDENRODYELLOW = 0xfafad2
SEAGREEN = 0x2e8b57
DIMGREY = 0x696969
LIGHTGRAY = 0xd3d3d3
LIGHTBLUE = 0xadd8e6
FLORALWHITE = 0xfffaf0
LAVENDERBLUSH = 0xfff0f5
OLDLACE = 0xfdf5e6
INDIGO = 0x4b0082
CORNFLOWERBLUE = 0x6495ed
GRAY = 0x808080
LIGHTYELLOW = 0xffffe0
DARKSLATEGRAY = 0x2f4f4f
TURQUOISE = 0x40e0d0
CADETBLUE = 0x5f9ea0
LAVENDER = 0xe6e6fa
LINEN = 0xfaf0e6
NAVAJOWHITE = 0xffdead
MEDIUMSLATEBLUE = 0x7b68ee
AQUAMARINE = 0x7fffd4
LIGHTSALMON = 0xffa07a
ROYALBLUE = 0x4169e1
PINK = 0xffc0cb
DEEPPINK = 0xff1493
MEDIUMSEAGREEN = 0x3cb371
BLUEVIOLET = 0x8a2be2
CRIMSON = 0xdc143c
CORAL = 0xff7f50
BLACK = 0x0
MEDIUMBLUE = 0xcd
BLUE = 0xff
SILVER = 0xc0c0c0
PALEGREEN = 0x98fb98
MEDIUMAQUAMARINE = 0x66cdaa
HOTPINK = 0xff69b4
BROWN = 0xa52a2a
LEMONCHIFFON = 0xfffacd
GOLDENROD = 0xdaa520
MEDIUMSPRINGGREEN = 0xfa9a
MINTCREAM = 0xf5fffa
