#!/usr/bin/env python3

import sys

from digitama.forward import *
from digitama.world import *
from digitama.graphics.font import *
from digitama.graphics.colorspace import *

from digitama.matter.graphlet.shapelet import *
from digitama.matter.graphlet.ui.textlet import *

###################################################################################################
class Gallery(World):
    def __init__(this):
        super(Gallery, this).__init__("The Gallery of Sprites and Graphlets")
        this.label = None
        this.lines = []
        this.rectangle = None
        this.circle = None
        this.ellipse = None
        this.regular_polygons = []

    def load(this, width, height):
        this.label = this.insert(Labellet("精灵和图元陈列馆", game_font.unicode, BLACK))

        # 三角形的三条边
        this.lines.append(this.insert(Linelet(-200, +200, RED)))
        this.lines.append(this.insert(Linelet(+400, +000, GREEN)))
        this.lines.append(this.insert(Linelet(+200, +200, BLUE)))

        this.rectangle = this.insert(Rectanglet(200, 100, YELLOWGREEN, ORANGE)) # 橘边黄绿色长方形
        this.circle = this.insert(Circlet(50, PINK, PURPLE))                    # 紫边粉红色圆
        this.ellipse = this.insert(Ellipselet(64, 32, KHAKI, SKYBLUE))          # 天蓝色边卡其色椭圆
    
        for n in range(1, 11):
            this.regular_polygons.append(this.insert(RegularPolygonlet(n, 32, ROYALBLUE, FIREBRICK, rotation = -90.0)))

    def reflow(this, width, height):
        # 排列线段以组成三角形
        this.move_to(this.lines[0], (200.0, 400.0), MatterAnchor.LB) # 左下角对齐
        this.move_to(this.lines[1], (200.0, 400.0), MatterAnchor.LT) # 左上角对齐
        this.move_to(this.lines[2], (600.0, 400.0), MatterAnchor.RB) # 右下角对齐

        # 排列长方形和椭圆
        this.move_to(this.rectangle, (800.0, 100.0), MatterAnchor.LT)
        this.move_to(this.circle, (900.0, 400.0), MatterAnchor.CC)
        this.move_to(this.ellipse, (900.0, 600.0), MatterAnchor.CC)

        # 排列正多边形
        for n in range(0, len(this.regular_polygons)):
            this.move_to(this.regular_polygons[n], (100.0 * float(n), 750.0), MatterAnchor.CC)

    def can_select(this, g): return True


###################################################################################################
if __name__ == '__main__':
    universe = Gallery()
    universe.big_bang()

    sys.exit(0)
