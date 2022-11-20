#include "paddleball_cosmos.hpp"

#include "digitama/graphlet/shapelet.hpp"

using namespace WarGrey::STEM;

const float ball_radius = 8.0F;
const float paddle_width = 128.0F;
const float paddle_height = 8.0F;

const float ball_speed = 4.0F;
const float paddle_speed = ball_speed * 3.0F;

/*************************************************************************************************/
void WarGrey::STEM::PaddleBallPlanet::load(float width, float height) {
    this->ball = this->insert_one(new Circlelet(ball_radius, ORANGE));
    this->paddle = this->insert_one(new Rectanglet(paddle_width, paddle_height, FORESTGREEN));

    this->ball->set_border_collision_strategy(BorderCollisionStrategy::BOUNCE, BorderCollisionStrategy::BOUNCE,
            BorderCollisionStrategy::STOP, BorderCollisionStrategy::BOUNCE);

    this->paddle->set_border_collision_strategy(BorderCollisionStrategy::IGNORE, BorderCollisionStrategy::STOP);
}

void WarGrey::STEM::PaddleBallPlanet::reflow(float width, float height) {
    // 确保球产生于屏幕上方的中间
    this->move_to(this->ball, width * 0.5F, ball_radius, GraphletAnchor::CT);
    this->ball->set_speed(ball_speed, ball_speed);
    this->ball->set_color(ORANGE);

    // 确保桨产生在靠近屏幕下方的中间
    this->move_to(this->paddle, width * 0.5F, height - paddle_height * 3.0F);
}

void WarGrey::STEM::PaddleBallPlanet::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    float paddle_lx, paddle_ty, paddle_rx, paddle_by;
    float ball_lx, ball_ty, ball_rx, ball_by;

    this->fill_graphlet_location(this->paddle, &paddle_lx, &paddle_ty, GraphletAnchor::LT);
    this->fill_graphlet_location(this->paddle, &paddle_rx, &paddle_by, GraphletAnchor::RB);
    
    this->fill_graphlet_location(this->ball, &ball_lx, &ball_ty, GraphletAnchor::LT);
    this->fill_graphlet_location(this->ball, &ball_rx, &ball_by, GraphletAnchor::RB);

    if (ball_ty < paddle_by) { // 球未脱板, 检测小球是否被捕获
        if ((ball_by >= paddle_ty) && (ball_lx >= paddle_lx) && (ball_rx <= paddle_rx)) {
            this->ball->motion_bounce(false, true);
        }
    } else {
        this->ball->set_color(RED);
    }
}

/*************************************************************************************************/
void WarGrey::STEM::PaddleBallPlanet::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    switch(key) {
        case 'a': this->paddle->set_speed((pressed ? -paddle_speed : 0.0F), 0.0F); break;
        case 'd': this->paddle->set_speed((pressed ? +paddle_speed : 0.0F), 0.0F); break;
    }
}

