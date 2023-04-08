#!/usr/bin/env python3

from digitama.big_bang.game import *    # 导入游戏模块，内含 Plane 类和常用函数

###############################################################################
# 本游戏中的常量设定
step_size = 2.0
step_duration = 0.2

###############################################################################
class DrunkardWalkWorld(Plane):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题和帧频
        super(DrunkardWalkWorld, self).__init__("醉汉漫步")

        # 本游戏世界有以下物体
        self.drunkard = None
        self.partner = None
        self.beach = None
        self.tent = None

    def load(self, Width, Height):
        self.beach = self.insert(Sprite(digimon_path("asset/beach", ".png")))
        self.drunkard = self.insert(Agate())
        self.partner = self.insert(Tita())

    def reflow(self, width, height):
        self.move_to(self.beach, (width * 0.5, height), MatterAnchor.CB)

    def update(self, count, interval, uptime):
        if not self.is_colliding(self.drunkard, self.partner):
            if self.partner.motion_stopped():
                self.__sibling_walk()

            self.__drunkard_walk()
        else:
            self.partner.motion_stop()
            self.drunkard.switch_mode(BracerMode.Win, 1)
            self.partner.switch_mode(BracerMode.Win, 1)

    def can_select(self, matter):
        return True
    
# protected
    def on_mission_start(self, width, height):
        self.drunkard.switch_mode(BracerMode.Walk)
        self.drunkard.set_heading(-180.0)

        self.move_to(self.drunkard, (width * 0.95, height * 0.9), MatterAnchor.CC)
        self.move_to(self.partner, (width * 0.24, height * 0.9), MatterAnchor.LC)

# private
    def __sibling_walk(self):
        dx = random.randint(-1, 1)
        dy = random.randint(-1, 1)

        self.glide(step_duration, self.partner, dx * step_size, dy * step_size)

    def __drunkard_walk(self):
        chance = random.randint(1, 100)
        dx = 0.0
        dy = 0.0
        
        if chance < 10:
            pass # no move    
        elif chance < 58:
            dx = -1.0
        elif chance < 60:
            dx = +1.0
        elif chance < 80:
            dy = +1.0
        else:
            dy = -1.0

        self.move(self.drunkard, dx, dy)

###############################################################################
launch_universe(DrunkardWalkWorld, __name__)
