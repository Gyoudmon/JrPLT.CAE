from digitama.game import *         # 游戏模块
from digitama.geometry import *     # 基本图形模块

from digitama.font import *         # 导入预定义字体
from digitama.text import *         # 还用到了“画文字”函数

###############################################################################
# 定义一个类型，并命名为 Ball（球）
class Ball(object):
    def __init__(self):
        # 球的位置
        self.x = 0
        self.y = 0

        # 球的速度
        self.dx = 0
        self.dy = 0

# 定义一个字典，用来搜集 Paddle（桨）的属性
def make_paddle():
    # 桨的位置，以键值对的形式表达
    return { 'x': 0, 'y': 0, 'speed': 0 }

###############################################################################
ball_radius = 8
paddle_width = 128
paddle_height = 8

ball_speed = 4
paddle_speed = ball_speed * 3

class PaddleBallGame(Universe):
    def __init__(self):
        # 通过父类的构造函数设置窗口标题
        super(PaddleBallGame, self).__init__("Paddle Ball", 60, 0x000000, 0xFFFFFF)

        self.ball = Ball()
        self.paddle = make_paddle()
        
        self.screen_width = 0
        self.screen_height = 0

    def reflow(self, width, height):
        self.screen_width = width
        self.screen_height = height

        # 确保球产生与屏幕上方的中间
        self.ball.x = self.screen_width // 2
        self.ball.y = ball_radius

        self.ball.dx = ball_speed * 1
        self.ball.dy = ball_speed * 1

        # 确保桨产生在靠近屏幕下方的中间
        self.paddle['x'] = self.ball.x - paddle_width // 2
        self.paddle['y'] = self.screen_height - paddle_height * 3

    def update(self, interval, count, uptime):
        dead_y = self.screen_height - ball_radius

        if self.ball.y < dead_y: # 球未脱板
            # 移动球，碰到左右边界、上边界反弹
            self.ball.x = self.ball.x + self.ball.dx
            self.ball.y = self.ball.y + self.ball.dy

            if self.ball.x <= ball_radius or self.ball.x >= self.screen_width - ball_radius:
                self.ball.dx = -self.ball.dx

            if self.ball.y <= ball_radius:
                self.ball.dy = -self.ball.dy

            # 移动桨，碰到边界停止
            if self.paddle['speed'] != 0:
                self.paddle['x'] += self.paddle['speed']

                if self.paddle['x'] < 0:
                    self.paddle['x'] = 0
                elif self.paddle['x'] + paddle_width > self.screen_width:
                    self.paddle['x'] = self.screen_width - paddle_width

            # 检测小球是否被捕获
            ball_bottom = self.ball.y + ball_radius

            if ball_bottom >= self.paddle['y'] and self.ball.x >= self.paddle['x'] and self.ball.x <= self.paddle['x'] + paddle_width:
                self.ball.dy = -self.ball.dy

    def draw(self, renderer, x, y, width, height):
        if self.ball.y >= self.paddle['y']:
            ball_color = RED
        else:
            ball_color = ORANGE

        game_fill_circle(renderer, self.ball.x, self.ball.y, ball_radius, ball_color)
        game_fill_rect(renderer, self.paddle['x'], self.paddle['y'], paddle_width, paddle_height, FORESTGREEN)

    def _on_char(self, key, modifiers, repeats, pressed):
        match key:
            case 'a':
                if pressed:
                    self.paddle['speed'] = -paddle_speed
                else:
                    self.paddle['speed'] = 0
            case 'd':
                if pressed:
                    self.paddle['speed'] = +paddle_speed
                else:
                    self.paddle['speed'] = 0

