from digitama.game import *
from digitama.matter.graphlet.shapelet import *

################################ 定义游戏世界的常量 ##############################
ball_radius = 8.0
paddle_width = 128.0
paddle_height = 8.0

ball_speed = 6.0
paddle_speed = ball_speed * 1.5

###############################################################################
class PaddleBallWorld(World):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题
        super(PaddleBallWorld, self).__init__("Paddle Ball")

        # 本游戏世界中的物体
        self.ball = None
        self.paddle = None

    # 实现 PaddleBallWorld::load 方法，加载球和桨，设置相关边界碰撞策略和速度
    def load(self, width, height):
        self.ball = self.insert(Circlet(ball_radius, ORANGE))
        self.paddle = self.insert(Rectanglet(paddle_width, paddle_height, FORESTGREEN))

        self.ball.set_border_strategy((BorderStrategy.BOUNCE, BorderStrategy.BOUNCE, BorderStrategy.STOP, BorderStrategy.BOUNCE))
        self.paddle.set_border_strategy((BorderStrategy.IGNORE, BorderStrategy.STOP))

        self.ball.set_speed(ball_speed, 45.0)

    # 实现 PaddleBallWorld::reflow 方法，调整球和桨的初始位置
    def reflow(self, width, height):
        # 确保球产生与屏幕上方的中间
        self.move_to(self.ball, (width * 0.5, ball_radius), MatterAnchor.CT)
        # 确保桨产生在靠近屏幕下方的中间
        self.move_to(self.paddle, (width * 0.5, height - paddle_height * 3.0), MatterAnchor.CC)

    # 实现 PaddleBallWorld::update 方法，根据当前球和桨的当前位置判断是否有碰撞，无需考虑运动细节
    def update(self, interval, count, uptime):
        paddle_lx, paddle_ty = self.get_matter_location(self.paddle, MatterAnchor.LT)
        paddle_rx, paddle_by = self.get_matter_location(self.paddle, MatterAnchor.RB)

        ball_lx, ball_ty = self.get_matter_location(self.ball, MatterAnchor.LT)
        ball_rx, ball_by = self.get_matter_location(self.ball, MatterAnchor.RB)

        if ball_ty < paddle_by: # 球未脱板, 检测小球是否被捕获
            if ball_by >= paddle_ty and ball_by <= paddle_by and ball_lx >= paddle_lx and ball_rx <= paddle_rx:
                self.ball.motion_bounce(False, True) # 正常，反弹球
        else:
            self.ball.set_color(RED)

    # 实现 PaddleBallWorld::on_char 方法，处理键盘事件，用于控制桨的移动
    def _on_char(self, key, modifiers, repeats, pressed):
        if key == 'a':
            if pressed:
                self.paddle.set_speed(paddle_speed, 180.0)
            else:
                self.paddle.set_speed(0.0, 180.0)
        elif key == 'd':
            if pressed:
                self.paddle.set_speed(paddle_speed, 0.0)
            else:
                self.paddle.set_speed(0.0, 0.0)
