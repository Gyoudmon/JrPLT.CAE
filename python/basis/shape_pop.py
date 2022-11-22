### shape.py
from digitama.world import *         # 导入 World 类
from digitama.geometry import *      # 导入"画图形“模块
from digitama.text import *          # 导入“画文字”模块

from digitama.font import *          # 导入预定义字体

###############################################################################
# 创建自定义数据类型，并命名为 ShapeWorld，继承自 World
class ShapeWorld(World):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题和帧频
        super(ShapeWorld, self).__init__("Primitive Shapes", 0)

    # 实现 ShapeWorld::construct 函数，设置窗口大小
    def construct(self, argv):
        self.set_window_size(1200, 800)

    # 实现 ShapeWorld::draw 函数，本例中绘制一系列几何图形
    def draw(self, renderer, x, y, width, height):
        game_draw_blended_text(game_font.unicode, renderer,
            self.get_foreground_color(), 10, 10, "基本图形陈列馆(过程式)")

        game_draw_point(renderer, 600, 300, ROYALBLUE)              # 画皇家蓝点
    
        ### 绘制三角形 ###
        game_draw_line(renderer, 400, 200, 200, 400, RED)           # 画红色线段
        game_draw_line(renderer, 200, 400, 600, 400, GREEN)         # 画绿色线段
        game_draw_line(renderer, 600, 400, 400, 200, BLUE)          # 画蓝色线段

        ### 绘制矩形 ###
        game_fill_rect(renderer, 800, 100, 200, 100, YELLOWGREEN)   # 画黄绿色矩形
        game_draw_rect(renderer, 800, 100, 200, 100, ORANGE)        # 画橘色边框

        ### 绘制圆 ###
        game_fill_circle(renderer, 900, 400, 50, PINK)              # 画粉红色圆
        game_draw_circle(renderer, 900, 400, 50, PURPLE)            # 画紫色轮廓

        ### 绘制椭圆 ###
        game_fill_ellipse(renderer, 900, 600, 64, 32, KHAKI)        # 画卡其色椭圆
        game_draw_ellipse(renderer, 900, 600, 64, 32, SKYBLUE)      # 画天蓝色轮廓

        for n in range(1, 11):
            game_fill_regular_polygon(renderer, n, 100 * (n - 1), 750, 32, -90.0, ROYALBLUE)
            game_draw_regular_polygon(renderer, n, 100 * (n - 1), 750, 32, -90.0, FIREBRICK)

