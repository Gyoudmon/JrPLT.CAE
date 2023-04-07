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

    def reflow(self, width, height):
        self.move_to(self.beach, (width * 0.5, height), MatterAnchor.CB)

    def can_select(self, matter):
        return True

# private
    def __sibling_walk(self):
        pass

    def __drunkard_walk(self):
        pass

###############################################################################
launch_universe(DrunkardWalkWorld, __name__)
