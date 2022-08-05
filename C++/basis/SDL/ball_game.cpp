#include "ball_game.hpp"

using namespace WarGrey::STEM;

const int ball_radius = 8;
const int paddle_width = 32;
const int paddle_height = 4;
const int ball_speed = 4;

/*************************************************************************************************/
WarGrey::STEM::BallGame::BallGame(int width, int height)
    : Universe("Ball Game", width, height, 30, 0x000000FFU, 0xFFFFFFFFU) { /* 什么都不做 */ }

WarGrey::STEM::BallGame::~BallGame() { /* 什么都不做 */ }

/*************************************************************************************************/
void WarGrey::STEM::BallGame::construct(int argc, char* argv[]) {
    this->fill_window_size(&this->screen_width, &this->screen_height);

    // 确保球产生于屏幕上方的中间
    this->ball_x = this->screen_width / 2;
    this->ball_y = ball_radius;

    this->ball_dx = 1 * ball_speed;
    this->ball_dy = 1 * ball_speed;

    // 确保浆产生在屏幕下方的中间
    this->paddle_x = this->ball_x - paddle_width / 2;
    this->paddle_y = this->screen_height - paddle_height;
}

void WarGrey::STEM::BallGame::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    this->ball_x = this->ball_x + this->ball_dx;
    this->ball_y = this->ball_y + this->ball_dy;

    if ((this->ball_x <= ball_radius) || (this->ball_x >= this->screen_width - ball_radius)) {
        this->ball_dx = -this->ball_dx;
    }
    
    if ((this->ball_y <= ball_radius) || (this->ball_y >= this->screen_height - ball_radius)) {
        this->ball_dy = -this->ball_dy;
    }
}

void WarGrey::STEM::BallGame::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_RenderDrawLine(renderer, this->ball_x, this->ball_y, this->paddle_x, this->paddle_y);
    game_fill_circle(renderer, this->ball_x, this->ball_y, ball_radius, 0x00FF00FFU);
    game_fill_rectangle(renderer, this->paddle_x, this->paddle_y, paddle_width, paddle_height, 0x00FFFFU);
}

/*************************************************************************************************/
void WarGrey::STEM::BallGame::on_char(char key, uint16_t modifiers, uint8_t repeats) {
    switch(key) {
        case 'a': {
            if (this->paddle_x > 0) {
                this->paddle_x -= 1;
            }
        }; break;
        case 'd': {
            if (this->paddle_x < this->screen_width - paddle_width) {
                this->paddle_x += 1;
            }
        }; break;
    }
}

