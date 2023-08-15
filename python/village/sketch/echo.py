import sys # 系统相关参数和函数

# 程序入口函数，不一定非得命名为 main, 此处仅为与习惯对比
def main(参数数量, 参数小组):
    print("Received %s arguments:" % 参数数量)

    # 显示所有用户输入参数，一行一个
    for i in range(0, 参数数量):
        print("    参数[%s]: %s" % (i, 参数小组[i]))

    return 0

sys.exit(main(len(sys.argv), sys.argv))
