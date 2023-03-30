from .forward import *
from .graphics.image import *
from .physics.motion import *
from .virtualization.iscreen import *

###############################################################################
class IMatterInfo(object):
    def __init__(self, master):
        self.master = master

class IMatter(IMovable):
    def __init__(self):
        super(IMatter, self).__init__()
        self.info = None
        self.__resizable = False
        self.__anchor, self.__anchor_x, self.__anchor_y = MatterAnchor.LT, 0.0, 0.0
        self.__deal_with_events, self.__deal_with_low_level_events = False, False
        self.__findable = True
        self.__invisible = False

    def __del__(self):
        self.info = None

    def master(self):
        plane = None

        if self.info:
            plane = self.info.master

        return plane

# public
    def construct(self): pass
    def get_extent(self, x, y): return 0.0, 0.0
    def get_margin(self, x, y): return 0.0, 0.0, 0.0, 0.0
    def update(self, count, interval, uptime): pass
    def draw(self, renderer, X, Y, Width, Height): pass
    def ready(self): return True

# public
    def own_caret(self, x, y): pass

    def has_caret(self):
        careted = False

        if self.info:
            careted = (self.info.master.get_focused_matter == self)

        return careted

# public
    def is_colliding_with_mouse(self, local_x, local_y): return True
    def on_char(self, key, modifiers, repeats, pressed): return False
    def on_hover(self, local_x, local_y): return False
    def on_tap(self, local_x, local_y): return False
    def on_goodbye(self, local_x, local_y): return False

# public
    def on_pointer_pressed(self, button, x, y, clicks, touch): return False
    def on_pointer_released(self, button, x, y, clicks, touch): return False
    def on_pointer_move(self, state, x, y, dx, dy, touch): return False
    
# public
    def on_location_changed(self, x, y, old_x, old_y): pass

# public
    def enable_events(self, yes_or_no, low_level = False):
        self.__deal_with_events = yes_or_no
        self.__deal_with_low_level_events = low_level
    
    def enable_resizing(self, yes_no):
        self.__resizable = yes_no

    def resizable(self):
        return self.__resizable

    def events_allowed(self):
        return self.__deal_with_events

    def low_level_events_allowed(self):
        return self.events_allowed() and self.__deal_with_low_level_events

# public
    def get_location(self, anchor = MatterAnchor.LT):
        sx = 0.0
        sy = 0.0

        if self.info:
            sx, sy = self.info.master.get_matter_location(self, anchor)

        return sx, sy

    def moor(self, anchor):
        if anchor != MatterAnchor.LT:
            if self.info:
                self.__anchor = anchor
                self.__anchor_x, self.__anchor_y = self.info.master.get_matter_location(self, self.__anchor)

    def clear_moor(self):
        self.__anchor = MatterAnchor.LT
        self.__anchor_x, self.__anchor_y = 0.0, 0.0
    
    def notify_updated(self):
        if self.info:
            if self.__anchor != MatterAnchor.LT:
                cx, cy = self.info.master.get_matter_location(self, self.__anchor)

                if (self.__anchor_x != cx) or (self.__anchor_y != cy): 
                    self.info.master.move(self, (self.__anchor_x - cx, self.__anchor_y - cy), True)
                
                self.clear_moor()
            
            self.info.master.notify_updated()

    def resize(self, w, h, anchor = MatterAnchor.CC):
        if self.__resizable:
            if w > 0.0 and h > 0.0:
                x, y = self.get_location(MatterAnchor.LT)
                width, height = self.get_extent(x, y)

                if width != w or height != h:
                    self.moor(anchor)
                    self._on_resize(w, h, width, height)
                    self.notify_updated()
    
    def camouflage(self, yes_or_no):
        self.__findable = yes_or_no
    
    def concealled(self):
        return not self.__findable
    
    def show(self, yes_or_no):
        if self.__invisible == yes_or_no:
            self.__invisible = (not yes_or_no)
            self.notify_updated()

    def visible(self):
        return not self.__invisible

# proteceted
    def _on_resized(self, width, height, old_width, old_height):
        pass
