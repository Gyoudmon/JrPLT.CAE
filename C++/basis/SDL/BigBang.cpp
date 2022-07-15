#include "game_of_life.hpp"

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

