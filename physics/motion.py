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
        self.__tvx, self.__tvy = math.inf, math.inf

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
        elif hstrategy == BorderStrategy.BOUNCE or vstrategy == BorderStrategy.BOUNCE:
            self.motion_bounce(hstrategy == BorderStrategy.BOUNCE, vstrategy == BorderStrategy.BOUNCE)

# public
    def set_acceleration(self, acc, direction, is_radian = False):
        ax, ay = orthogonal_decomposition(acc, direction, is_radian)
        self.set_delta_speed(ax, ay)

    def add_acceleration(self, acc, direction, is_radian = False):
        ax, ay = orthogonal_decomposition(acc, direction, is_radian)
        self.add_delta_speed(ax, ay)

    def get_acceleration(self):
        return vector_magnitude(self.__ax, self.__ay)

    def get_acceleration_direction(self, need_radian = True):
        rad = self.__ar

        if math.isnan(rad):
            rad = math.atan2(self.__ay, self.__ax)

        if not need_radian:
            rad = radians_to_degrees(rad)

        return rad

    def set_delta_speed(self, xacc, yacc):
        xchanged = (self.__ax != xacc)
        ychanged = (self.__ay != yacc)

        if xchanged: self.__ax = xacc
        if ychanged: self.__ay = yacc

        if xchanged or ychanged:
            self.__on_acceleration_changed()

    def add_delta_speed(self, xacc, yacc):
        self.set_delta_speed(self.__ax + xacc, self.__ay + yacc)

    def x_delta_speed(self):
        return self.__ax
    
    def y_delta_speed(self):
        return self.__ay

# public
    def set_velocity(self, spd, direction, is_radian = False):
        vx, vy = orthogonal_decomposition(spd, direction, is_radian)
        self.set_speed(vx, vy)

    def add_velocity(self, spd, direction, is_radian = False):
        vx, vy = orthogonal_decomposition(spd, direction, is_radian)
        self.add_speed(vx, vy)

    def get_velocity(self):
        return vector_magnitude(self._vx, self.__vy)
    
    def get_velocity_direction(self, need_radian = True):
        rad = self.__vr

        if math.isnan(rad):
            rad = math.atan2(self.__vy, self.__vx)

        if not need_radian:
            rad = radians_to_degrees(rad)

        return rad
        
    def set_speed(self, xspd, yspd):
        if (xspd > self.__tvx): xspd = self.__tvx
        if (yspd > self.__tvy): yspd = self.__tvy

        self.__vx = xspd
        self.__vy = yspd
        self.__on_velocity_changed()

    def add_speed(self, xspd, yspd):
        pass

    def x_speed(self):
        return self.__vx
    
    def y_speed(self):
        return self.__vy

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
