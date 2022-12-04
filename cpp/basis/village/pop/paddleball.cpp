#include "paddleball.hpp"

using namespace WarGrey::STEM;

const float ball_radius = 8.0F;
const float paddle_width = 128.0F;
const float paddle_height = 8.0F;

const float ball_speed = 4.0F;
const float paddle_speed = ball_speed * 3.0F;

/*************************************************************************************************/
// 实现 PaddleBallWorld::reflow 方法，调整球和桨的初始位置
void WarGrey::STEM::PaddleBallWorld::reflow(float width, float height) {
    // 确保球产生于屏幕上方的中间
    this->ball.x = width / 2.0F;
    this->ball.y = ball_radius;

    this->ball.dx = 1.0F * ball_speed;
    this->ball.dy = 1.0F * ball_speed;

    // 确保桨产生在靠近屏幕下方的中间
    this->paddle.x = this->ball.x - paddle_width / 2.0F;
    this->paddle.y = height - paddle_height * 3.0F;
}

// 实现 PaddleBallWorld::draw 方法，在舞台上绘制出当前位置的球和桨
void WarGrey::STEM::PaddleBallWorld::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    game_fill_circle(renderer, this->ball.x, this->ball.y, ball_radius, this->ball.color);
    game_fill_rect(renderer, this->paddle.x, this->paddle.y, paddle_width, paddle_height, FORESTGREEN);
}

// 实现 PaddleBallWorld::update 方法，刷新球和桨的位置，这就是“运动动画”的基本原理
void WarGrey::STEM::PaddleBallWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    int width, height;
    this->feed_window_size(&width, &height);

    if (this->ball.y < height - ball_radius) { // 球未触底
        /* 移动球，碰到边界反弹 */ {
            this->ball.x = this->ball.x + this->ball.dx;
            this->ball.y = this->ball.y + this->ball.dy;

            if ((this->ball.x <= ball_radius) || (this->ball.x >= width - ball_radius)) {
                this->ball.dx = -this->ball.dx;
            }
    
            if (this->ball.y <= ball_radius) {
                this->ball.dy = -this->ball.dy;
            }
        }

        /* 移动桨，碰到边界停止 */ {
            if (this->paddle.speed != 0.0F) {
                this->paddle.x += this->paddle.speed;

                if (this->paddle.x < 0.0F) {
                    this->paddle.x = 0.0F;
                } else if (this->paddle.x + paddle_width > width) {
                    this->paddle.x = width - paddle_width;
                }
            }
        }

        /* 检测小球是否被捕获 */ {
            if (this->ball.y + ball_radius >= this->paddle.y) {
                if ((this->ball.x >= this->paddle.x) && (this->ball.x <= this->paddle.x + paddle_width)) {
                    this->ball.dy = -this->ball.dy;
                } else {
                    ball.color = RED;
                }
            }
        }

        this->notify_updated(); // 更新有效，通知系统舞台需要重绘
    }
}

// 实现 PaddleBallWorld::on_char 方法，处理键盘事件，用于控制桨的移动
void WarGrey::STEM::PaddleBallWorld::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    switch(key) {
        case 'a': this->paddle.speed = (pressed ? -paddle_speed : 0.0F); break;
        case 'd': this->paddle.speed = (pressed ? +paddle_speed : 0.0F); break;
    }
}
