#include "game_of_life.hpp"

#include "text.hpp"
#include "random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const int GRID_SIZE = 16;     // 方格边长
static const int WORLD_SIZE = 48;    // 方格单边数量

typedef struct game_world {
    int grids[WORLD_SIZE][WORLD_SIZE];
    int draw_x;
    int draw_y;
    int lineheight;
} game_world_t;

game_world_t* game_world_construct(int width, int height, int nx, int ny) {
    int world_length = GRID_SIZE * WORLD_SIZE;
    game_world_t* world = new game_world();

    // 确保游戏世界被绘制在屏幕中心
    world->draw_x = (width - world_length) / 2;
    world->draw_y = (height - world_length) / 2;

    game_text_size(game_monospace_font, NULL, &world->lineheight, "em");

    return world;
}

void game_world_destroy(game_world_t* world) {
    delete world;
}

/*************************************************************************************************/
void* game_of_life_initialize(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    int world_length = GRID_SIZE * WORLD_SIZE;
    int width, height;
    game_world_t* world;

    SDL_SetWindowTitle(window, "Game of Life");                 // 设置标题
    SDL_GetWindowSize(window, &width, &height);                 // 获知窗口大小
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);   // 设置混色模式, 不要混色！！！
    
    return game_world_construct(width, height, WORLD_SIZE, WORLD_SIZE);
}

void update_game_of_life(timer_frame_t* frame, void* datum, SDL_Renderer* renderer) {
    game_world_t* world = (game_world_t*)datum;

    // 绘制游戏世界的网格
    game_draw_grid(renderer, WORLD_SIZE, WORLD_SIZE, GRID_SIZE, world->draw_x, world->draw_y);

    // 绘制路径
    game_fill_grid(renderer, (int *)world->grids, WORLD_SIZE, WORLD_SIZE, GRID_SIZE, world->draw_x, world->draw_y);
}

void game_of_life_exit(void* datum) {
    game_world_destroy((game_world_t*)datum);
}

