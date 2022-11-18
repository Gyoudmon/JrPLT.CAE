#!/usr/bin/env python3

from paddleball import *  # 导入当前要启动的游戏

import sys

# __name__ 是一个特殊变量
# 可用于提示是否从这行代码启动应用程序
# 效果上相当于 C++ 的 main 函数，用以启动游戏

if __name__=="__main__":

    # 混沌初开，宇宙诞生，游戏世界就绪
    # Python 设计在惯例上无需 C++ 那样的“两步初始化”
    universe = PaddleBallGame()

    # 宇宙大爆炸
    # 开启游戏主循环，直到玩家关闭游戏
    universe.big_bang()

    # Python 心满意足地退出
    # 顺便销毁游戏宇宙，回归虚无
    sys.exit(0)


