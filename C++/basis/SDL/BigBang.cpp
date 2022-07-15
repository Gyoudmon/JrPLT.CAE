/* 自己的头文件用 双引号 引用, 放最前面以兼容 macOS */
#include "game_of_life.hpp"

#include <iostream>             /* C++ 标准输入输出头文件 */

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const int WIN_WIDTH  = 1200;
static const int WIN_HEIGHT = 800;

/*************************************************************************************************/
int main(int argc, char* args[]) {
    /* 创建自己的游戏宇宙 */
    auto universe = new ConwayLife(WIN_WIDTH, WIN_HEIGHT);

    /* 创建游戏世界 */
    universe->construct(argc, args);

    /* 开始游戏主循环 */
    universe->big_bang();

    /* 结束之前别忘了销毁游戏宇宙 */
    delete universe;

    return 0;
}

