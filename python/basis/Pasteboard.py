#!/usr/bin/env python3

import sys           # 系统相关参数和函数

from shape import *  # 当前要启动的游戏

# __name__ 是一个特殊变量
# 可用于提示是否从这行代码启动应用程序
# 效果上相当于 C++ 的 main 函数，用以启动游戏

if __name__=="__main__":

    # 混沌初开，宇宙诞生
    universe = PrimitiveShape(1200, 800)

    # 创建游戏世界
    universe.construct(sys.argv)

    # 宇宙大爆炸
    # 开启游戏主循环，直到玩家关闭游戏
    universe.big_bang()

    # Python 心满意足地退出
    # 顺便销毁游戏宇宙，回归虚无
    sys.exit(0)


