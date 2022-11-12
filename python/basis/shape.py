from digitama.game import *          # 游戏模块
from digitama.geometry import *      # 基本图形模块

###############################################################################
# 创建自定义数据类型，并命名为 PrimitiveShape，继承自 Pasteboard
class PrimitiveShape(Pasteboard):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题
        super(PrimitiveShape, self).__init__("Primitive Shapes")

        # 设置窗口大小
        self.set_window_size(1200, 800)

    def draw(self, renderer, x, y, width, height):
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
        #game_fill_ellipse(renderer, 900, 600, 64, 32, KHAKI)        # 画卡其色椭圆
        #game_draw_ellipse(renderer, 900, 600, 64, 32, SKYBLUE)      # 画天蓝色轮廓


