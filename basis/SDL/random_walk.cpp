#include <SDL2/SDL.h>  /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */

#include "random_walk.h"
#include "random.h"

static int x;
static int y;
static int width;
static int height;

/*************************************************************************************************/
void* random_walk_initialize(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetWindowTitle(window, "Random Walk");                // 设置标题
    SDL_GetWindowSize(window, &width, &height);               // 获知窗口大小
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); // 设置混色模式, 不要混色！！！
    
    // 初始位置在屏幕中心
    x = width / 2;
    y = height / 2;

    return NULL;
}

void update_random_walk(unsigned int count, unsigned int interval, unsigned long long uptime,
        void* datum, SDL_Renderer* renderer) {
    if ((x > 0) && (x < width) && (y > 0) && (y < height)) {
        /**
         * random_raw() 随机产生一个整数
         * 执行 ‘% 3’ 得到 0, 1, 或 2 三个数
         * 再 ‘- 1’ 得到 -1, 0, 1 三个数作为坐标的增量
         **/
        x += (random_raw() % 3 - 1); // 左右移动或不动
        y += (random_raw() % 3 - 1); // 上下移动或不动

        SDL_RenderDrawPoint(renderer, x, y);
    }
}

