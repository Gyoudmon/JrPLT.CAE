#include "paddleball.hpp"

using namespace WarGrey::STEM;

const float ball_radius = 8.0F;
const float paddle_width = 128.0F;
const float paddle_height = 8.0F;

const float ball_speed = 6.0F;
const float paddle_speed = ball_speed * 1.5F;

/*************************************************************************************************/
// 实现 PaddleBallWorld::load 方法，加载球和桨，设置相关边界碰撞策略和速度
void WarGrey::STEM::PaddleBallWorld::load(float width, float height) {
    TheBigBang::load(width, height);

    this->ball = this->insert(new Circlet(ball_radius, ORANGE));
    this->paddle = this->insert(new Rectanglet(paddle_width, paddle_height, FORESTGREEN));

    this->ball->set_border_strategy(BorderStrategy::BOUNCE, BorderStrategy::BOUNCE, BorderStrategy::STOP, BorderStrategy::BOUNCE);
    this->paddle->set_border_strategy(BorderStrategy::IGNORE, BorderStrategy::STOP);
    
    this->ball->set_speed(ball_speed, 45.0F);
}

// 实现 PaddleBallWorld::reflow 方法，调整球和桨的位置
void WarGrey::STEM::PaddleBallWorld::reflow(float width, float height) {
    // 确保球产生于屏幕上方的中间
    this->move_to(this->ball, width * 0.5F, ball_radius, MatterAnchor::CT);
    
    // 确保桨产生在靠近屏幕下方的中间
    this->move_to(this->paddle, width * 0.5F, height - paddle_height * 3.0F, MatterAnchor::CC);
}

// 实现 PaddleBallWorld::update 方法，根据当前球和桨的当前位置判断是否有碰撞，无需考虑运动细节
void WarGrey::STEM::PaddleBallWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    float paddle_lx, paddle_ty, paddle_rx, paddle_by;
    float ball_lx, ball_ty, ball_rx, ball_by;

    // 依次查询桨左上角和右下角的位置
    this->feed_matter_location(this->paddle, &paddle_lx, &paddle_ty, MatterAnchor::LT);
    this->feed_matter_location(this->paddle, &paddle_rx, &paddle_by, MatterAnchor::RB);
    
    // 依次查询球左上角和右下角的位置
    this->feed_matter_location(this->ball, &ball_lx, &ball_ty, MatterAnchor::LT);
    this->feed_matter_location(this->ball, &ball_rx, &ball_by, MatterAnchor::RB);

    if (ball_ty < paddle_by) { // 球未脱板, 检测小球是否被捕获
        if ((ball_by >= paddle_ty) && (ball_by <= paddle_by)
                && (ball_lx >= paddle_lx) && (ball_rx <= paddle_rx)) {
            this->ball->motion_bounce(false, true); // 正常，反弹球
        }
    } else {
        this->ball->set_color(RED);
    }
}

// 实现 PaddleBallWorld::on_char 方法，处理键盘事件，用于控制桨的移动
void WarGrey::STEM::PaddleBallWorld::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    switch(key) {
        case 'a': this->paddle->set_speed(pressed ? paddle_speed : 0.0F, 180.0F); break;
        case 'd': this->paddle->set_speed(pressed ? paddle_speed : 0.0F, 000.0F); break;
    }
}
