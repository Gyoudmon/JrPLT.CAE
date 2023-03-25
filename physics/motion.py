from ..forward import *

from .mathematics import *

###############################################################################
class IMovable(object):
    def __init__(self):
        super(IMovable, self).__init__()
        
        self.__border_strategies = {}
        self.__bounce_acc = False
        self.__ar, self.__ax, self.__ay = 0.0, 0.0, 0.0
        self.__vr, self.__vx, self.__vy = 0.0, 0.0, 0.0
        self.__tvx, self.__tvy = 0.0, 0.0

        self.set_border_strategy(BorderStrategy.IGNORE)
        self.motion_stop(True, True)

# public
    def on_border(self, hoffset, voffset):
        hstrategy = BorderStrategy.IGNORE
        vstrategy = BorderStrategy.IGNORE

        if hoffset < 0.0:
            hstrategy = self.__border_strategies[BorderEdge.LEFT]
        elif hoffset > 0.0:
            hstrategy = self.__border_strategies[BorderEdge.RIGHT]

        if voffset < 0.0:
            vstrategy = self.__border_strategies[BorderEdge.TOP]
        elif voffset > 0.0:
            vstrategy = self.__border_strategies[BorderEdge.BOTTOM]

        if hstrategy == BorderStrategy.STOP or vstrategy == BorderStrategy.STOP:
            self.motion_stop(True, True) # if stopping, both direction should stop

# public
    def set_acceleration(self, acc, direction, is_radian = False): pass
    def add_acceleration(self, acc, direction, is_radian = False): pass
    def get_acceleration(self): pass
    def get_acceleration_direction(self, need_radian = True): pass

    def set_delta_speed(self, xacc, yacc): pass
    def add_delta_speed(self, xacc, yacc): pass
    def x_delta_speed(self):
        return self.ax
    
    def y_delta_speed(self):
        return self.ay

# public
    def set_velocity(self, spd, direction, is_radian = False): pass
    def add_velocity(self, spd, direction, is_radian = False): pass
    def get_velocity(self): pass
    def get_velocity_direction(self, need_radian = True): pass
        
    def set_speed(self, xspd, yspd): pass
    def add_speed(self, xspd, yspd): pass
    def x_speed(self):
        return self.vx
    
    def y_speed(self):
        return self.vy

# public
    def set_terminal_velocity(self, max_spd, direction, is_radian = False): pass
    def set_terminal_speed(self, mxspd, myspd):
        self.__tvx = mxspd
        self.__tvy = myspd

    def get_heading(self, need_radian = True):
        return self.get_velocity_direction()
    
    def heading_rotate(self, theta, is_radian = False): pass

# public
    def set_border_strategy(self, s): pass
    def set_border_strategy(self, vs, hs): pass
    def set_border_strategy(self, ts, rs, bs, ls): pass

# public
    def step(self, sx, sy): pass
    def motion_stop(self, horizon = True, vertical = True): pass
    def motion_bounce(self, horizon, vertical): pass
    def disable_acceleration_bounce(self, yes = True):
        self.__bounce_acc = (not yes)
    
# public
    def x_stopped(self):
        return self.__ax == 0.0 and self.__vx == 0.0

    def y_stopped(self):
        return self.__ay == 0.0 and self.__vy == 0.0
    
    def motion_stopped(self):
        return self.x_stopped() and self.y_stopped()

# protected
    def _on_heading_changed(self, theta_rad, vx, vy, prev_vr): pass
    def _on_motion_stopped(self): pass

# private
    def __on_acceleration_changed(self): pass
    def __check_velocity_changing(self): pass
    def __on_velocity_changed(self): pass
