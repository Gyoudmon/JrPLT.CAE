#include "paddleball_pop.hpp"

using namespace WarGrey::STEM;

const int ball_radius = 8;
const int paddle_width = 128;
const int paddle_height = 8;

const int ball_speed = 4;
const int paddle_speed = ball_speed * 3;

/*************************************************************************************************/
void WarGrey::STEM::PaddleBallUniverse::reflow(int width, int height) {
    this->screen_width = width;
    this->screen_height = height;

    // 确保球产生于屏幕上方的中间
    this->ball.x = this->screen_width / 2;
    this->ball.y = ball_radius;

    this->ball.dx = 1 * ball_speed;
    this->ball.dy = 1 * ball_speed;

    // 确保桨产生在靠近屏幕下方的中间
    this->paddle.x = this->ball.x - paddle_width / 2;
    this->paddle.y = this->screen_height - paddle_height * 3;
}

void WarGrey::STEM::PaddleBallUniverse::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    int dead_y = this->screen_height - ball_radius;

    if (this->ball.y < dead_y) { // 球未脱板
        /* 移动球，碰到边界反弹 */ {
            this->ball.x = this->ball.x + this->ball.dx;
            this->ball.y = this->ball.y + this->ball.dy;

            if ((this->ball.x <= ball_radius) || (this->ball.x >= this->screen_width - ball_radius)) {
                this->ball.dx = -this->ball.dx;
            }
    
            if (this->ball.y <= ball_radius) {
                this->ball.dy = -this->ball.dy;
            }
        }

        /* 移动桨，碰到边界停止 */ {
            if (this->paddle.speed != 0) {
                this->paddle.x += this->paddle.speed;

                if (this->paddle.x < 0) {
                    this->paddle.x = 0;
                } else if (this->paddle.x + paddle_width > screen_width) {
                    this->paddle.x = screen_width - paddle_width;
                }
            }
        }

        /* 检测小球是否被捕获 */ {
            int ball_bottom = this->ball.y + ball_radius;

            if ((ball_bottom >= this->paddle.y)
                    && (this->ball.x >= this->paddle.x)
                    && (this->ball.x <= this->paddle.x + paddle_width)) {
                this->ball.dy = -this->ball.dy;
            }
        }
    }
}

void WarGrey::STEM::PaddleBallUniverse::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    uint32_t ball_color = ORANGE;

    if (this->ball.y >= this->paddle.y) {
        ball_color = RED;
    }

    game_fill_circle(renderer, this->ball.x, this->ball.y, ball_radius, ball_color);
    game_fill_rect(renderer, this->paddle.x, this->paddle.y, paddle_width, paddle_height, FORESTGREEN);
}

/*************************************************************************************************/
void WarGrey::STEM::PaddleBallUniverse::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    switch(key) {
        case 'a': this->paddle.speed = (pressed ? -paddle_speed : 0); break;
        case 'd': this->paddle.speed = (pressed ? +paddle_speed : 0); break;
    }
}

