#include "drunkard.hpp"

#include "../big_bang/physics/random.hpp"

using namespace WarGrey::STEM;

static const float step_size = 2.0F;
static const double step_duration = 0.2;

/*************************************************************************************************/
void WarGrey::STEM::DrunkardWalkWorld::load(float width, float height) {
    this->beach = this->insert(new Sprite(digimon_path("asset/beach", ".png")));
    this->tent = this->insert(new SpriteGridSheet(digimon_path("asset/tents", ".png"), 1, 4));
    this->drunkard = this->insert(new Agate());
    this->partner = this->insert(new Tita());

    TheBigBang::load(width, height);
}

void WarGrey::STEM::DrunkardWalkWorld::reflow(float width, float height) {
    this->move_to(this->beach, width * 0.5F, height, MatterAnchor::CB);
    this->move_to(this->tent, 0.0F, height, MatterAnchor::LB);
    
    TheBigBang::reflow(width, height);
}

void WarGrey::STEM::DrunkardWalkWorld::on_mission_start(float width, float height) {
    this->drunkard->switch_mode(BracerMode::Walk);
    this->drunkard->set_heading(-180.0);

    this->move_to(this->drunkard, width * 0.95F, height * 0.9F, MatterAnchor::CC);
    this->move_to(this->partner, width * 0.24F, height * 0.9F, MatterAnchor::CC);
}

void WarGrey::STEM::DrunkardWalkWorld::update(uint64_t interval, uint32_t count, uint64_t uptime) {
    if (!this->is_colliding(this->drunkard, this->partner)) {
        if (this->partner->motion_stopped()) {
            this->sibling_walk();
        }

        this->drunkard_walk();
    } else if (this->partner->current_mode() != BracerMode::Win) {
        this->partner->motion_stop();
        this->drunkard->switch_mode(BracerMode::Win, 1);
        this->partner->switch_mode(BracerMode::Win, 1);
    }
}

/*************************************************************************************************/
void WarGrey::STEM::DrunkardWalkWorld::sibling_walk() {
    // random_uniform(-1, 1) 长生一个位于区间 [-1, 1] 的随机数
    int dx = (random_uniform(-1, 1)); // 左右移动或不动
    int dy = (random_uniform(-1, 1)); // 上下移动或不动

    this->glide(step_duration, this->partner, dx * step_size, dy * step_size);
}

void WarGrey::STEM::DrunkardWalkWorld::drunkard_walk() {
    // 产生位于区间 [1, 100] 的随机数
    int chance = random_uniform(1, 100);
    double dx = 0.0;
    double dy = 0.0;
    
    if (chance < 10) {
        // no move
    } else if (chance < 58) {
        dx = -1.0;
    } else if (chance < 60) {
        dx = +1.0;
    } else if (chance < 80) {
        dy = +1.0;
    } else {
        dy = -1.0;
    }

    this->move(this->drunkard, dx, dy);
}
