#include <SDL2/SDL.h>  /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */
#include <cstdlib>

#include "drunkard_walk.h"

static int x;
static int y;
static int width;
static int height;

/*************************************************************************************************/
void* drunkard_walk_initialize(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetWindowTitle(window, "Drunkard Walk");              // 设置标题
    SDL_GetWindowSize(window, &width, &height);               // 获知窗口大小
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); // 设置混色模式, 不要混色！！！
    
    // 初始位置在屏幕中心
    x = width / 2;
    y = height / 2; 

    return NULL;
}

void update_drunkard_walk(unsigned int count, unsigned int interval, unsigned long long uptime, void* datum, SDL_Renderer* renderer) {
    if ((x > 0) && (x < width) && (y > 0) && (y < height)) {
        int chance = rand() % 100 + 1; // 产生位于区间 [1, 100] 的随机数

        if (chance < 40) {
            x--; // x = x - 1;
        } else if (chance < 60) {
            x++; // x = x + 1;
        } else if (chance < 80) {
            y++; // y = y + 1;
        } else {
            y--; // y = y - 1;
        }

        SDL_RenderDrawPoint(renderer, x, y);
    }
}

