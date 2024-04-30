#!/usr/bin/env python3

from digitama.big_bang.game import *    # 导入游戏模块，内含 Plane 类和常用函数

# 本游戏中的常量设定
radius: float = 100.0
gliding_duration: float = 2.0

###############################################################################
class CYMMixtureWorld(Plane):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题和帧频
        super(CYMMixtureWorld, self).__init__("减色模型")

        # 本游戏世界有以下物体
        self.cyan = None
        self.yellow = None
        self.magenta = None
        self.background = None

    def load(self, Width, Height):
        # 思考：为什么背景一定要是白色的？
        self.set_background(0xFFFFFF)

        self.cyan = self.insert(Circlet(radius, 0xFF0000))    # 青色，红色互补色
        self.yellow = self.insert(Circlet(radius, 0x00FF00))  # 黄色，绿色互补色
        self.magenta = self.insert(Circlet(radius, 0x0000FF)) # 品红色，蓝色互补色
   
        # 设置混色模式，CYM 属减色模型
        self.cyan.set_color_mixture(ColorMixture.Subtract)
        self.yellow.set_color_mixture(ColorMixture.Subtract)
        self.magenta.set_color_mixture(ColorMixture.Subtract)

    def reflow(self, width, height):
        self.move_to(self.yellow, (width, height * 0.5), MatterAnchor.RC)
        self.move_to(self.cyan, (self.yellow, MatterAnchor.CT), MatterAnchor.CB)
        self.move_to(self.magenta, (self.yellow, MatterAnchor.CB), MatterAnchor.CT)

    def can_select(self, matter):
        return isinstance(matter, Circlet)

    # 实现 CYMMixture::after_select 方法
    def after_select(self, matter, yes):
        if not yes:
            self.glide_to_mouse(gliding_duration, matter, MatterAnchor.CC)
        else:
            self.send_to_back(matter)

    def on_tap_selected(self, matter, x, y):
        self.no_selected()
    
###############################################################################
launch_universe(CYMMixtureWorld, __name__)
