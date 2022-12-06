#!/usr/bin/env python3

import sys # 系统相关参数和函数

def main(argc, argv):
    """ 程序入口函数，不一定非得命名为 main, 此处仅为与习惯对比

    :param argc: 'argument count' 的缩写, 即'参数个数'的意思
    :param argv: 'argument vector' 的缩写, 即'参数数组'的意思，包含命令行的所有输入参数
    """

    # 第一个参数一定是程序的名字(也就是这个源码文件的名字)
    print("the running program is " + argv[0])
    print("  received %s arguments from user" % (argc - 1))

    # 显示所有用户输入参数，一行一个
    for i in range(1, argc):
        print("    argv[%s]: %s" % (i, argv[i]))

    return 0



# __name__ 是一个特殊变量
# 可用于提示是否从这行代码启动应用程序
# 效果上相当于 C++ 程序的启动器，负责调用 main 函数
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))


