from .virtualization.iscreen import *
from .matter import *

###############################################################################
class IPlanetInfo(object):
    def __init__(self, master):
        super(IPlanetInfo, self).__init__()
        self.master = master

class Plane(object):
    def __init__(self, name, initial_mode = 0):
        super(Plane, self).__init__()
        self.info = None
        self._caption = name
        self._mode = initial_mode
        self._background, self._bg_alpha = -1, 1.0
        self._mleft, self._mtop, self._mright, self._mbottom = 0.0, 0.0, 0.0, 0.0
        self._head_matter, self._focused_matter, self._hovering_matter = None, None, None
        self._translate_x, self._translate_y = 0.0, 0.0
        self._scale_x, self._scale_y = 1.0, 1.0

    def __del__(self): pass

# public
    def change_mode(self, mode): pass
    def current_mode(self): return self._mode
    def matter_unmasked(self, m): pass

# public
    def construct(self, Width, Height): pass
    def load(self, Width, Height): pass
    def reflow(self, width, height): pass
    def update(self, count, interval, uptime): pass
    def draw(self, renderer, X, Y, Width, Height): pass
    def can_exit(self): return False

# public
    def find_matter(self, x, y): pass
    def get_matter_location(self, m, fx, fy): pass
    def get_matter_boundary(self, m): pass
    def get_matters_boundary(self): pass
    def insert_at(self, m, target, anchor, delta): pass
    def move(self, m, x, y): pass
    def move_to(self, m, target, anchor, delta): pass
    def remove(self, m): pass
    def erase(self): pass
    def size_cache_invalid(self): pass

# public
    def draw_visible_selection(self, renderer, x, y, width, height): pass
    def find_next_selected_matter(self, start = None): pass
    def thumbnail_matter(self): pass
    def add_selected(self, m): pass
    def set_selected(self, m): pass
    def no_selected(self): pass
    def count_selected(self): pass
    def is_selected(self, m): pass

    def set_background(self, chex, a = 1.0):
        self._background = chex
        self._bg_alpha = a

    def feed_background(self, sdlc): pass

    def start_input_text(self, prompt): pass
    def log_message(self, message): pass

# public
    def on_pointer_pressed(self, button, x, y, clicks, touch): return False
    def on_pointer_released(self, button, x, y, clicks, touch): return False
    def on_pointer_move(self, state, x, y, dx, dy, touch): return False
    def on_scroll(self, horizon, vertical, hprecise, vprecise): return False

# public
    def on_focus(self, m, on_off): pass
    def on_char(self, key, modifiers, repeats, pressed): pass
    def on_text(self, text, size, entire): pass
    def on_editing_text(self, text, pos, span): pass
    def on_elapse(self, count, interval, uptime): pass
    def on_hover(self, m, local_x, local_y): pass
    def on_goodbye(self, m, local_x, local_y): pass
    def on_tap(self, m, local_x, local_y): pass
    def on_tap_selected(self, m, local_x, local_y): pass
    def on_save(self): pass

# public
    def can_interactive_move(self, m, local_x, local_y): return False
    def can_select(self, m): return False
    def can_select_multiple(self): return False
    def before_select(self, m, on_or_off): pass
    def after_select(self, m, on_or_off): pass
        
# public
    def get_focused_matter(self): pass
    def set_caret_owner(self, m): pass
    def notify_matter_ready(self, m): pass
    def on_matter_ready(self, m): pass

# public
    def begin_update_sequence(self): pass
    def in_update_sequence(self): pass
    def end_update_sequence(self): pass
    def needs_update(self): pass
    def notify_updated(self): pass

# public
    def snapshot(self, width, height, bgcolor = 0, alpha = 0.0): pass
    def snapshot(self, x, y, width, height, bgcolor = 0, alpha = 0.0): pass
        
    def save_snapshot(self, pname, width, height, bgcolor = 0, alpha = 0.0): pass
    def save_snapshot(self, pname, x, y, width, height, bgcolor = 0, alpha = 0.0): pass

# private
    def recalculate_matters_extent_when_invalid(self): pass
    def say_goodbye_to_hover_matter(self, state, x, y, dx, dy): pass
    def collapse(self): pass

###################################################################################################
class __MatterInfo(IMatterInfo):
    def __init__(self, master, mode):
        super(__MatterInfo, self).__init__(master)
        
        self.mode = mode
        self.x, self.y = 0.0, 0.0
        self.selected = False
        self.iasync = {}
        
        self.next, self.prev = None, None

def __bind_matter_owership(master, mode, m):
    info = __MatterInfo(master, mode)

    m.info = info

    return info

def __unsafe_matter_unmasked(info, mode):
    return (info.mode & mode) == info.mode

def __unsafe_get_matter_bound(m, info):
    width, height = m.get_extent(info.x, info.y)

    return info.x, info.y, width, height

def __unsafe_add_selected(master, m, info):
    master.before_select(m, True)
    info.selected = True
    master.after_select(m, True)
    master.notify_updated()

def __unsafe_set_selected(master, m, info):
    master.begin_update_sequence()
    master.no_selected()
    __unsafe_add_selected(master, m, info)
    master.end_update_sequence()

def __matter_anchor_fraction(a):
    fx, fy = 0.0, 0.0

    if a == MatterAnchor.LT: pass
    elif a == MatterAnchor.LC: fy = 0.5
    elif a == MatterAnchor.LB: fy = 1.0
    elif a == MatterAnchor.CT: fx = 0.5          
    elif a == MatterAnchor.CC: fx, fy = 0.5, 0.5
    elif a == MatterAnchor.CB: fx, fy = 0.5, 1.0
    elif a == MatterAnchor.RT: fx = 1.0
    elif a == MatterAnchor.RC: fx, fy = 1.0, 0.5
    elif a == MatterAnchor.RB: fx, fy = 1.0, 1.0

    return fx, fy

def __unsafe_do_moving_via_info(master, info, x, y, absolute):
    moved = False

    if not absolute:
        x += info.x
        y += info.y

    if info.x != x or info.y != y:
        info.x = x
        info.y = y

        master.size_cache_invalid()
        moved = True

    return moved

def __unsafe_move_matter_via_info(master, m, info, x, y, fx, fy, dx, dy, absolute):
    ax, ay = 0.0, 0.0

    if m.ready():
        sx, sy, sw, sh = __unsafe_get_matter_bound(m, info)
        ax = sw * fx
        ay = sh * fy
    else:
        info.iasync['x0'] = x
        info.iasync['y0'] = y
        info.iasync['fx0'] = fx
        info.iasync['fy0'] = fy
        info.iasync['dx0'] = dx
        info.iasync['dy0'] = dy

    return __unsafe_do_moving_via_info(master, info, x - ax + dx, y - ay + dy, True)

def __do_search_selected_matter(start, mode, terminator):
    found = None
    child = start

    while child != terminator:
        info = child.info

        if info.selected and __unsafe_matter_unmasked(info, mode):
            found = child
            break

        child = info.next

    return found

def __do_resize(master, m, info, scale_x, scale_y, prev_scale_x = 1.0, prev_scale_y = 1.0):
    resizable, resize_anchor = m.resizable()

    if resizable:
        sx, sy, sw, sh = __unsafe_get_matter_bound(m, info)
        fx, fy = __matter_anchor_fraction(resize_anchor)

        m.resize(sw / prev_scale_x * scale_x, sh / prev_scale_y * scale_y)
        nw, nh = m.get_extent(sx, sy)

        nx = sx + (sw - nw) * fx
        ny = sy + (sh - nh) * fy

        __unsafe_do_moving_via_info(master, info, nx, ny, True)

