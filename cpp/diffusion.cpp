#include "diffusion.hpp"

#include "digitama/big_bang/physics/random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const int DROP_WIDTH = 64;

/*************************************************************************************************/
WarGrey::STEM::Diffusion::Diffusion() : Universe("Diffusion", 24, 0x000000U, 0xFFFFFFU) {}

void WarGrey::STEM::Diffusion::reflow(int width, int height) {
    this->ceiling = height / 4;

    for (int i = 0; i < PARTICLE_COUNT; i++) {
        // 产生位于区间 [-DROP_WIDTH/2, DROP_WIDTH/2) 的偏移量
        int offset = i % DROP_WIDTH  - DROP_WIDTH / 2;

        // 初始化数组
        this->xs[i] = width / 2 + offset;
        this->ys[i] = this->ceiling;
    }
}

void WarGrey::STEM::Diffusion::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        int chance = random_uniform(1, 100); // 产生位于区间 [1, 100] 的随机数

        if (chance < 30) {
            this->ys[i]++;
        } else if (chance < 55) {
            this->xs[i]++;
        } else if (chance < 80) {
            this->xs[i]--;
        } else if (ys[i] > ceiling) {
            this->ys[i]--;
        }
    }
}

void WarGrey::STEM::Diffusion::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        // 访问数组元素
        if ((this->xs[i] > 0) && (this->xs[i] < width) && (this->ys[i] > 0) && (this->ys[i] < height)) {
            game_fill_circle(renderer, this->xs[i], this->ys[i], 1, this->get_foreground_color());
        }
    }
}
