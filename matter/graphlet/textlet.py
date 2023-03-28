from ..igraphlet import *

from ...forward import *
from ...graphics.colorspace import *
from ...graphics.named_colors import *
from ...graphics.font import *
from ...graphics.text import *

###################################################################################################
class ITextlet(IGraphlet):
    def __init__(self):
        super(ITextlet, self).__init__()
        self.__raw = ""
        self._text_color = SILVER
        self._alpha = 1.0
        self._text_font = None
        self._text_surface = None
        self.set_text_color()

    def __def__(self):
        if self._text_surface:
            del self._text_surface
            self._text_surface = None

# public
    def set_text_color(self, color_hex = SILVER, alpha = 1.0):
        if self._text_color != color_hex or self._alpha != alpha:
            self._text_color = color_hex
            self._alpha = alpha
            self.__update_text_surface()
            self.notify_updated()

    def set_font(self, font, anchor = MatterAnchor.LT):
        self.moor(anchor)

        if font:
            self._text_font = font
        else:
            self._text_font = game_font.DEFAULT

        self._on_font_changed()
        self.notify_updated()

    def set_text(self, content, anchor = MatterAnchor.LT):
        self.__raw = content
        self.moor(anchor)

        if self._text_font:
            self.__update_text_surface()
        else:
            self.set_font(None, anchor)

        self.notify_updated()

# public
    def get_extent(self, x, y):
        w, h = 0.0, 0.0

        if self._text_surface:
            w, h = self._text_surface.get_size()
        else:
            w, h = super(ITextlet, self).get_extent(x, y)

        return w, h

    def draw(self, renderer, x, y, Width, Height):
        if self._text_surface:
            game_render_surface(renderer, self._text_surface, (x, y))

# protected
    def _on_font_changed(self): pass

# private
    def __update_text_surface(self):
        if self._text_surface:
            del self._text_surface

        if self.__raw:
            self._text_surface = game_text_surface(self.__raw, self._text_font,
                TextRenderMode.Blender, self._text_color, self._text_color, 0)
        else:
            self._text_surface = None            

###################################################################################################
class Labellet(ITextlet):
    def __init__(self, caption, font, color_hex = -1, alpha = 1.0):
        super(Labellet, self).__init__()

        if font:
            self.set_font(font)
        
        if color_hex >= 0:
            self.set_text_color(color_hex, alpha)

        self.set_text(caption)
