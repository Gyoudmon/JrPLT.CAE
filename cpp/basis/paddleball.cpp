#include "paddleball.hpp"

using namespace WarGrey::STEM;

const int ball_radius = 8;
const int paddle_width = 128;
const int paddle_height = 8;

const int ball_speed = 4;
const int paddle_speed = ball_speed * 3;

/*************************************************************************************************/
WarGrey::STEM::PaddleBallGame::PaddleBallGame()
    : Universe("Paddle Ball", 60, 0x000000U, 0xFFFFFFU) { /* 什么都不做 */ }

WarGrey::STEM::PaddleBallGame::~PaddleBallGame() { /* 什么都不做 */ }

/*************************************************************************************************/
void WarGrey::STEM::PaddleBallGame::reflow(int width, int height) {
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

void WarGrey::STEM::PaddleBallGame::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    int dead_y = this->screen_height - ball_radius;

    if (this->ball.y < dead_y) {
        this->ball.x = this->ball.x + this->ball.dx;
        this->ball.y = this->ball.y + this->ball.dy;

        if ((this->ball.x <= ball_radius) || (this->ball.x >= this->screen_width - ball_radius)) {
            this->ball.dx = -this->ball.dx;
        }
    
        if (this->ball.y <= ball_radius) {
            this->ball.dy = -this->ball.dy;
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

void WarGrey::STEM::PaddleBallGame::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    game_fill_circle(renderer, this->ball.x, this->ball.y, ball_radius,
            ((this->ball.y >= this->paddle.y) ? RED : ORANGE));

    game_fill_rect(renderer, this->paddle.x, this->paddle.y,
            paddle_width, paddle_height, FORESTGREEN);
}

/*************************************************************************************************/
void WarGrey::STEM::PaddleBallGame::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    if (pressed) {
        switch(key) {
            case 'a': {
                if (this->paddle.x > 0) {
                    this->paddle.x -= paddle_speed;
                    // this->notify_updated();
                }
            }; break;
            case 'd': {
                if ((this->paddle.x + paddle_width) < this->screen_width) {
                    this->paddle.x += paddle_speed;
                    // this->notify_updated();
                }
            }; break;
        }
    }
}

