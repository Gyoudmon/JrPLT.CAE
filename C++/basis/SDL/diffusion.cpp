#include "diffusion.hpp"

#include "random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static int width;
static int height;
static int ceiling;

static const int DROP_WIDTH = 32;
static const int PARTICLE_COUNT = 128;

// 声明数组
static int xs[PARTICLE_COUNT];
static int ys[PARTICLE_COUNT];

/*************************************************************************************************/
void* diffusion_initialize(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetWindowTitle(window, "Diffusion");                    // 设置标题
    SDL_GetWindowSize(window, &width, &height);                 // 获知窗口大小
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);   // 设置混色模式, 不要混色！！！

    ceiling = height / 4;

    for (int i = 0; i < PARTICLE_COUNT; i++) {
        int offset = i % DROP_WIDTH  - DROP_WIDTH / 2; // 产生位于区间 [-DROP_WIDTH/2, DROP_WIDTH/2) 的偏移量

        // 初始化数组
        xs[i] = width / 2 + offset;
        ys[i] = ceiling;
    }

    return NULL;
}

void update_diffusion(WarGrey::STEM::timer_frame_t* frame, void* datum, SDL_Renderer* renderer) {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        // 访问数组元素
        if ((xs[i] > 0) && (xs[i] < width) && (ys[i] > 0) && (ys[i] < height)) {
            int chance = random_uniform(1, 100); // 产生位于区间 [1, 100] 的随机数

            if (chance < 30) {
                ys[i]++;
            } else if (chance < 55) {
                xs[i]++;
            } else if (chance < 80) {
                xs[i]--;
            } else if (ys[i] > ceiling) {
                ys[i]--;
            }

            SDL_RenderDrawPoint(renderer, xs[i], ys[i]);
        }
    }
}

void diffusion_exit(void* datum) {
    // 无事可做
}

