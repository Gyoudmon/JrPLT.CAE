import colorsys

from game import *          # 游戏开发课程相关函数
from geometry import *      # 基本图形绘制函数

###############################################################################
class PrimitiveShape(DrawingBoard):
    def __init__(self, argv, width, height):
        super(PrimitiveShape, self).__init__(argv, "Primitive Shapes", width, height)

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


