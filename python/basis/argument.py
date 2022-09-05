#!/usr/bin/env python3

import sys # 系统相关参数和函数
import os  # 操作系统相关函数

def main(argv):
    ''' 程序入口函数，不一定非得命名为 main，但习惯上应该如此
    :param argv: "argument vector" 的缩写, 即"参数数组"的意思，包含命令行的所有输入参数
    '''

    # 第一个参数一定是程序的名字(也就是这个模块的名字)
    print(argv[0])
    print("  received {} arguments from user".format(len(argv) - 1))

    # 现实所有用户输入参数，一行一个
    for i in range(1, len(argv)):
        print("    argv[{}]: {}".format(i, argv[i]))

    return 0


# __name__ 是一个特殊变量
# 可用于提示是否从这行代码启动应用程序
# 效果上相当于 C++ 的 main 函数
if __name__=="__main__":
    os._exit(main(sys.argv))

