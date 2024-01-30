#!/usr/bin/env python3

### empty.py
from digitama.big_bang.game import *    # 导入游戏模块，内含 Plane 类和常用函数

###############################################################################
# 创建自定义数据类型，并命名为 EmptyWorld，继承自 Plane
class EmptyWorld(Plane):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题和帧频
        super(EmptyWorld, self).__init__("空宇宙")

        # 本游戏世界中有如下物体
        # (无)
        
###############################################################################
launch_universe(EmptyWorld, __name__)
