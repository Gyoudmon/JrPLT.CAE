#!/usr/bin/env python3

import sys # 系统相关参数和函数
import os  # 操作系统相关函数

def main(argc, argv):
    name = input("请告诉我你叫什么名字：")
    print("你好，" + name, end = "")
    coding_age = input("。那你学编程多少年啦？")
    print("好棒，已经 {} 年了呀。".format(coding_age), end = "")
    print("那么请继续精益求精！")

    return 0



###################################################################################################
if __name__=="__main__":
    os._exit(main(len(sys.argv), sys.argv))


