#include <SDL2/SDL.h>  /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */
#include <cstdlib>
#include <cstring>

#include "rgb_blocks.h"
#include "colorspace.h"

using namespace WarGrey::STEM;

/*************************************************************************************************/
// 色块边长常量
const int BLOCK_LENGTH = 200;

// 色块变量（矩形）
SDL_Rect b1, b2, b3;

int draw_contrast_blocks(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    int width, height;
    float hue0 = 0.0F;
    char title[256];
    
    if (argc > 1) {
        hue0 = float(std::atof(args[1]));
    }

    { // 设置标题
        size_t size = sizeof(title) / sizeof(char);

        memset(title, '\0', size);
        snprintf(title, size, "Contrast Color Blocks [hue = %.2f]", hue0);
        SDL_SetWindowTitle(window, title);
    }

    SDL_GetWindowSize(window, &width, &height);             // 获知窗口大小
    b1.w = b1.h = b2.w = b2.h = b3.w = b3.h = BLOCK_LENGTH; // 设置三个色块的尺寸

    /* 设置混色模式，RGB 是加色模型 */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    /* 绘制红色色块 */
    b1.x = (width - b1.w) / 2;
    b1.y = height / 2 - b1.h;
    HSV_SetRenderDrawColor(renderer, hue0 + 0.0F, 1.0F, 1.0F, 1.0F);
    SDL_RenderFillRect(renderer, &b1);

    /* 绘制绿色色块 */
    b2.x = width / 2 - b2.w + b2.w / 4;
    b2.y = height / 3;
    HSV_SetRenderDrawColor(renderer, hue0 + 120.0F, 1.0F, 1.0F, 1.0F);
    SDL_RenderFillRect(renderer, &b2);

    /* 绘制蓝色色块 */
    b3.x = width / 2 - b3.w / 4;
    b3.y = b2.y;
    HSV_SetRenderDrawColor(renderer, hue0 + 240.0F, 1.0F, 1.0F, 1.0F);
    SDL_RenderFillRect(renderer, &b3);

    return 0;
}

