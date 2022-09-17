from game import *   # 游戏开发课程相关函数

###############################################################################
class PrimitiveShape(DrawingBoard):
    def __init__(self, width, height):
        super(PrimitiveShape, self).__init__("Primitive Shapes", width, height)

    def draw(self, renderer, x, y, width, height):
        pass


