#include "random_walk.hpp"

#include "random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
WarGrey::STEM::RandomWalk::RandomWalk(const char* title) : DrawingPlayer(title) {}

void WarGrey::STEM::RandomWalk::reflow(int width, int height) {
    // 初始位置在屏幕中心
    this->x = width / 2;
    this->y = height / 2;
}

void WarGrey::STEM::RandomWalk::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    /**
     * random_raw() 随机产生一个整数
     * 执行 ‘% 3’ 得到 0, 1, 或 2 三个数
     * 再 ‘- 1’ 得到 -1, 0, 1 三个数作为坐标的增量
     **/
    x += (random_raw() % 3 - 1); // 左右移动或不动
    y += (random_raw() % 3 - 1); // 上下移动或不动
}

void WarGrey::STEM::RandomWalk::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    if ((this->x > 0) && (this->x < width) && (this->y > 0) && (this->y < height)) {
        SDL_RenderDrawPoint(renderer, this->x, this->y);
    }
}

/*************************************************************************************************/
WarGrey::STEM::DrunkardWalk::DrunkardWalk() : RandomWalk("Drunkard Walk") {}

void WarGrey::STEM::DrunkardWalk::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    int chance = random_raw() % 100 + 1; // 产生位于区间 [1, 100] 的随机数

    if (chance < 40) {
        this->x--; // x = x - 1;
    } else if (chance < 60) {
        this->x++; // x = x + 1;
    } else if (chance < 80) {
        this->y++; // y = y + 1;
    } else {
        this->y--; // y = y - 1;
    }
}

