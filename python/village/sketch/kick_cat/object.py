# 场景：踢猫效应

# 定义类和方法
class BadPerson(object):
    # 构造函数
    def __init__(self, name):
        # 坏人有名字，用于通报批评
        self.name = name
        print(f"好的，导演叫我扮演{name}")

    # 坏人会踢人
    # 方法令人不齿：欺软怕硬，欺负弱小
    # 不过，坏人一旦踢了人，大家就都知道坏人是谁了
    def kick(self, weak, this):
        # 根据方法签名，还原自然语言描述
        # I kick the cat with this foot
        print(f"{员工.name}用{this}脚踢了{weak}, {weak}很气愤")

# 实例化，找个人来扮演这个角色, 对象
老板 = BadPerson("画饼大师")
员工 = BadPerson("加班狗")
孩子 = BadPerson("国粹哥")

# 完整的“主谓宾”句子，还原“踢猫效应”情境
老板.kick(员工.name, "左")
员工.kick(孩子.name, "左")
孩子.kick("瘟猫", "左")
