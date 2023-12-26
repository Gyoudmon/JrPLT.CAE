# 场景：胡克定律

# 定义函数
# l(m) = 0.078m
def l(m):
    return 0.078 * m

# L(m, 原始长度, 限度) = 0.078 * m + 原始长度
def L(m, 原始长度, 限度):
    if m <= 限度:
        return 0.078 * m + 原始长度
    else:
        print(f"啊啊啊啊啊啊，弹簧坏掉啦！")
        return 0

# 调用函数：
答案1 = l(20)
答案2 = l(80)
c = L(45, 50, 100)
d = L(120, 50, 100)

print(答案1)
print(答案2)
print(c)
print(d)
