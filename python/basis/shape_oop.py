### shape.py
from digitama.world import *                # 导入 World 类

from digitama.graphlet.shapelet import *    # 导入基本图形模块
from digitama.graphlet.ui.textlet import *  # 导入UI文本模块

from digitama.font import *                 # 导入预定义字体

###############################################################################
# 创建自定义数据类型，并命名为 ShapeWorld，继承自 World
class ShapeWorld(World):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题和帧频
        super(ShapeWorld, self).__init__("Primitive Shapes", 0)

    # 实现 ShapeWorld::construct 函数，设置窗口大小
    def construct(self, argv):
        self.set_window_size(1200, 800)

    # 实现 ShapeWorld::load 方法，在舞台上加入基础几何图形的实例
    def load(self, Width, Height):
        # 请在此处添加加载几何图形对象的代码
        pass

    # 实现 ShapeWorld::reflow 方法，重新排列几何图形在舞台上的位置
    def reflow(self, Width, Height):
        # 请在此处添加排列几何图形对象的代码
        pass

