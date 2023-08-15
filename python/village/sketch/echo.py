import sys # 系统相关参数和函数

# 程序入口函数，不一定非得命名为 main, 此处仅为与习惯对比
def main(参数数量, 参数小组):
    print("Received %s arguments:" % 参数数量)

    # 显示所有用户输入参数，一行一个
    for i in range(0, 参数数量):
        print("    参数[%s]: %s" % (i, 参数小组[i]))

    return 0

# __name__ 是一个特殊变量
# 可用于提示是否从这行代码启动应用程序
# 效果上相当于 C++ 程序的启动器，负责调用 main 函数
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
