#!/usr/bin/env python3

from digitama.big_bang.game import *

################################ 定义游戏世界的常量 ##############################
ball_radius = 32.0
ball_speed = 4.0

###############################################################################
class MotionWorld(Plane):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题
        super().__init__("基本运动")

        # 本游戏世界中的物体
        self.red = None
        self.yellow = None
        self.blue = None
        self.green = None
        
    # 实现 MotionWorld::load 方法，加载四色小球
    def load(self, width, height):
        self.red = self.insert(Circlet(ball_radius, RED))
        self.yellow = self.insert(Circlet(ball_radius, YELLOW))
        self.blue = self.insert(Circlet(ball_radius, BLUE))
        self.green = self.insert(Circlet(ball_radius, GREEN))

        self.red.set_border_strategy([BorderStrategy.STOP])
        self.yellow.set_border_strategy([BorderStrategy.STOP])
        self.blue.set_border_strategy([BorderStrategy.STOP])
        self.green.set_border_strategy([BorderStrategy.STOP])

    # 实现 MotionWorld::on_mission_start 方法，调整球的位置和速度
    def reflow(self, width, height):
        self.move_to(self.red, (width * 0.5, ball_radius), MatterAnchor.CT)
        self.move_to(self.red, (width * 0.5, ball_radius), MatterAnchor.CT)
        self.move_to(self.yellow, (width - ball_radius, height * 0.5), MatterAnchor.RC)
        self.move_to(self.blue, (width * 0.5, height - ball_radius), MatterAnchor.CB)
        self.move_to(self.green, (ball_radius, height * 0.5), MatterAnchor.LC)

        self.red.set_speed(0.0, ball_speed)
        self.yellow.set_speed(-ball_speed, 0.0)
        self.blue.set_speed(0.0, -ball_speed)
        self.green.set_speed(ball_speed, 0.0)

###############################################################################
launch_universe(MotionWorld, __name__)
