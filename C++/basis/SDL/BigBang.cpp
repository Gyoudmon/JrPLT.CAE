#include "game_of_life.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const int WIN_WIDTH  = 1200;
static const int WIN_HEIGHT = 800;

/*************************************************************************************************/
int main(int argc, char* args[]) {
    /* 创建游戏宇宙 */
    auto universe = new ConwayLife(WIN_WIDTH, WIN_HEIGHT);

    /* 初始化游戏世界 */
    universe->construct(argc, args);

    /* 宇宙大爆炸，开启游戏主循环 */
    universe->big_bang();

    /* 销毁游戏宇宙，回归虚无 */
    delete universe;

    /* C++ 心满意足地退出 */
    return 0;
}

