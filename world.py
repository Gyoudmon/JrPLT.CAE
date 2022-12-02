from .cosmos import *

###############################################################################
class World(Cosmos):
    def __init__(this, title, fps = 60, fgc = 0x000000, bgc = 0xFFFFFF, initial_mode = 0):
        super(World, this).__init__(fps, fgc, bgc)
        this._plane = __WorldPlane(this, title, initial_mode)

# public
    def change_mode(this, mode):
        this._plane.change_mode(mode)

    def current_mode(this):
        return this._plane.current_mode()

    def matter_unmasked(this, m):
        return this._plane.matter_unmasked(m)

# public
    def construct(this, argv): pass
    def load(this, width, height): pass
    def reflow(this, width, height): pass

# public
    def can_interactive_move(this, m, local_x, local_y): return False
    def can_select(this, m): return False
    def can_select_multiple(this): return False
    def before_select(this, m, on_or_off): pass
    def after_select(this, m, on_or_off): pass

# public
    def get_matter_location(this, matter):
        return this._plane.get_matter_location(matter)

    def insert(this, matter, x, y, anchor, dx, dy):
        this._plane.insert(matter, x, y, anchor, dx, dy)

    def move(this, matter, x, y):
        this._plane.move(matter, x, y)

    def move_to(this, matter, target, anchor = MatterAnchor.LT, dx = 0.0, dy = 0.0):
        this._plane.move_to(matter, target, anchor, dx, dy)

    def remove(this, matter):
        this._plane.remove(matter)

    def erase(this):
        this._plane.erase()

###############################################################################
class __WorldPlane(Plane):
    def __init__(this, master, title, mode = 0):
        super(__WorldPlane, this).__init__(title, mode)
        this.__master = master

    def load(this, width, height):
        this.__master.load(width, height)

    def can_interactive_move(this, m, local_x, local_y):
        return this.__master.can_interactive_move(m, local_x, local_y)

    def can_select(this, m):
        return this.__master.can_select(m)

    def can_select_multiple(this):
        return this.__master.can_select_multiple()

    def before_select(this, m, on_or_off):
        return this.__master.before_select(m, on_or_off)

    def after_select(this, m, on_or_off):
        return this.__master.after_select(m, on_or_off)

