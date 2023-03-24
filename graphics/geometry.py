import sdl2                             # 原始 SDL2 函数
import sdl2.rect as sdlr                # 原始 SDL2 矩形结构体
import sdl2.sdlgfx as gfx               # SDL2 非官方几何图形库
import ctypes as ffi                    # 外语接口函数
import math                             # 基础数学函数

from .colorspace import *               # 色彩空间相关函数, 前面那个点指代相对本文件的路径
from ..physics.mathematics import *     # 图形学、线性代数相关函数

###############################################################################
def texture_size(texture):
    w = ffi.c_int()
    h = ffi.c_int()
    
    sdl2.SDL_QueryTexture(texture, None, None, ffi.byref(w), ffi.byref(h))
    
    return w.value, h.value

###############################################################################
def game_draw_point(renderer, x, y, cs, alpha = 0xFF):
    RGBA_SetRenderDrawColor(renderer, cs, alpha)
    sdl2.SDL_RenderDrawPoint(renderer, round(x), round(y))

def game_draw_line(renderer, x1, y1, x2, y2, cs, alpha = 0xFF):
    RGBA_SetRenderDrawColor(renderer, cs, alpha)
    sdl2.SDL_RenderDrawLine(renderer, round(x1), round(y1), round(x2), round(y2))

def game_draw_rect(renderer, x, y, width, height, cs, alpha = 0xFF):
    box = sdlr.SDL_Rect(round(x), round(y), round(width), round(height))
    
    RGBA_SetRenderDrawColor(renderer, cs, alpha)
    sdl2.SDL_RenderDrawRect(renderer, ffi.byref(box))

def game_fill_rect(renderer, x, y, width, height, cs, alpha = 0xFF):
    box = sdlr.SDL_Rect(round(x), round(y), round(width), round(height))
    
    RGBA_SetRenderDrawColor(renderer, cs, alpha)
    sdl2.SDL_RenderFillRect(renderer, ffi.byref(box))

def game_draw_square(renderer, cx, cy, apothem, cs, alpha = 0xFF):
    game_draw_rect(renderer, cx - apothem, cy - apothem, apothem * 2, apothem * 2, cs, alpha)

def game_fill_square(renderer, cx, cy, apothem, cs, alpha = 0xFF):
    game_fill_rect(renderer, cx - apothem, cy - apothem, apothem * 2, apothem * 2, cs, alpha)

def game_draw_circle(renderer, cx, cy, radius, cs, alpha = 0xFF):
    r, g, b, a = rgba(cs, alpha)
    gfx.aacircleRGBA(renderer, round(cx), round(cy), round(radius), r, g, b, a)

def game_fill_circle(renderer, cx, cy, radius, cs, alpha = 0xFF):
    r, g, b, a = rgba(cs, alpha)
    gfx.filledCircleRGBA(renderer, round(cx), round(cy), round(radius), r, g, b, a)

def game_draw_ellipse(renderer, cx, cy, aradius, bradius, cs, alpha = 0xFF):
    r, g, b, a = rgba(cs, alpha)
    gfx.aaellipseRGBA(renderer, round(cx), round(cy), round(aradius), round(bradius), r, g, b, a)

def game_fill_ellipse(renderer, cx, cy, aradius, bradius, cs, alpha = 0xFF):
    r, g, b, a = rgba(cs, alpha)
    gfx.filledEllipseRGBA(renderer, round(cx), round(cy), round(aradius), round(bradius), r, g, b, a)

def game_draw_regular_polygon(renderer, n, cx, cy, radius, rotation, cs, alpha = 0xFF):
    RGBA_SetRenderDrawColor(renderer, cs, alpha)
    _draw_regular_polygon(renderer, n, cx, cy, radius, rotation)

def game_fill_regular_polygon(renderer, n, cx, cy, radius, rotation, cs, alpha = 0xFF):
    RGBA_SetRenderDrawColor(renderer, cs, alpha)
    _fill_regular_polygon(renderer, n, cx, cy, radius, rotation)

###############################################################################
def game_render_surface(target, psurface, regions, flip = None, angle = 0.0):
    texture = sdl2.SDL_CreateTextureFromSurface(target, psurface)

    if texture:
        game_render_texture(target, texture, regions, flip, angle)
        sdl2.SDL_DestroyTexture(texture)

def game_render_texture(target, texture, regions, flip = None, angle = 0.0):
    '''
    :param target:  the instance of SDL_Renderer
    :param texture: the instance of SDL_Texture
    :param regions: hints to specify the rendering regions, which can be shaped as one of
                        (x, y)
                        (x, y, width, height)
                        SDL_Rect
                        (SDL_Rect, SDL_Rect)
    :param flip: the flip options
    :param angle: the rotation angle
    '''
    if isinstance(regions, sdlr.SDL_Rect):
        src, dst = None, regions
    elif len(regions) == 4:
        src, dst = None, sdlr.SDL_Rect(round(regions[0]), round(regions[1]), round(regions[2]), round(regions[3]))
    elif isinstance(regions[0], sdlr.SDL_Rect):
        src, dst = regions
    else:
        w, h = texture_size(texture)
        src, dst = None, sdlr.SDL_Rect(round(regions[0]), round(regions[1]), w, h)

    if (not flip) and (angle == 0.0):
        sdl2.SDL_RenderCopy(target, texture, src, dst)
    else:
        sdl2.SDL_RenderCopyEx(target, texture, src, dst, angle, None, flip)

###############################################################################
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
