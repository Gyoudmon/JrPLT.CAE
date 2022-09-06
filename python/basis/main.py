#!/usr/bin/env python3

import sys # 系统相关参数和函数
import os  # 操作系统相关函数

def main(argc, argv):
    print("Hello, World!")
    return 0



# __name__ 是一个特殊变量
# 可用于提示是否从这行代码启动应用程序
# 效果上相当于 C++ 程序的启动器，负责调用 main 函数
if __name__=="__main__":
    os._exit(main(len(sys.argv), sys.argv))


