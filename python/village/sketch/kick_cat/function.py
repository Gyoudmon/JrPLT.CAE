# 场景：踢猫效应

# 定义函数
def kick(cat, left):
    print(f"kick the {cat} with {left} foot")

# 调用，即使用函数
kick("员工", "左")
kick("小孩", "左")
kick("猫", "左")

# 定义“类”，舞台剧扮演的角色
class 坏人(object):
    # 构造函数
    def __init__(self, name):
        # 属性：名字
        self.name = name

    # 方法：踢
    def kick(self, cat, left):
        print(f"{self.name} kicks the {cat} with {left} foot")

# 实例化，找演员扮演上面定义的角色
老板 = 坏人("老板")
员工 = 坏人("员工")
孩子 = 坏人("孩子")

# 还原情景，完整“主谓宾”语句
孩子.kick("猫", "左")
