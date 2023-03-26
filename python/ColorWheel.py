#!/usr/bin/env python3

from digitama.big_bang.game import *    # 导入游戏模块，内含 Plane 类和常用函数

###############################################################################
# 本游戏中的常量设定
color_count = 36
color_radius = 16.0
wheel_radius = 360.0

mixture_radius: float = 100.0

###############################################################################
class ColorWheelWorld(Plane):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题和帧频
        super(ColorWheelWorld, self).__init__("色相环")

        # 本游戏世界有以下物体
        self.colors = []
        self.color_components = []

    def load(self, Width, Height):
        # 思考：为什么背景一定要是黑色的？
        self.set_background(0x000000)

        self.color_components.append(self.insert(Ellipselet(mixture_radius, mixture_radius, 0xFF0000))) # 红色
        self.color_components.append(self.insert(Ellipselet(mixture_radius, mixture_radius, 0x00FF00))) # 绿色
        self.color_components.append(self.insert(Ellipselet(mixture_radius, mixture_radius, 0x0000FF))) # 蓝色
   
        # 设置混色模式，RGB 属加色模型
        for cc in self.color_components:
            cc.set_color_mixture(ColorMixture.Add)

        self.__load_color_wheel_components()

    def reflow(self, width, height):
        cx = width * 0.5
        cy = height * 0.5
        delta_deg = 360.0 / float(color_count)
        deg = 0.0
        idx = 0

        while deg < 360.0:
            x, y = circle_point(wheel_radius, deg - 90.0, False)
            self.move_to(self.colors[idx], (cx + x, cy + y), MatterAnchor.CC)

            deg += delta_deg
            idx += 1

        self.__reflow_color_components(width * 0.5, height * 0.5)

    def can_select(self, matter):
        return isinstance(matter, Circlet)

    # 实现 ColorMixtureWorld::after_select 方法
    def after_select(self, matter, yes):
        pass

# private
    def __load_color_wheel_components(self):
        delta_deg = 360.0 / float(color_count)
        deg = 0.0

        while deg < 360.0:
            self.colors.append(self.insert(Circlet(color_radius, 0xFF00FF)))
            deg += delta_deg

    def __reflow_color_components(self, x, y):
        cc_off = mixture_radius * 0.5

        self.move_to(self.color_components[0], (x, y), MatterAnchor.CB, 0.0, cc_off)
        self.move_to(self.color_components[1], (self.color_components[0], MatterAnchor.CB), MatterAnchor.RC, cc_off)
        self.move_to(self.color_components[2], (self.color_components[1], MatterAnchor.CC), MatterAnchor.LC)

###############################################################################
launch_universe(ColorWheelWorld, __name__, (1200, 1200))
