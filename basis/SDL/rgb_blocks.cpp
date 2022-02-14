#include <SDL2/SDL.h>  /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */

#include "rgb_blocks.h"

/*************************************************************************************************/
// 色块边长常量
const int BLOCK_LENGTH = 200;

// 色块变量（矩形）
SDL_Rect red, green, blue;

int draw_rgb_blocks(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    int width, height;
    
    SDL_SetWindowTitle(window, "RGB Blocks");

    SDL_GetWindowSize(window, &width, &height);                         // 获知窗口大小
    red.w = red.h = green.w = green.h = blue.w = blue.h = BLOCK_LENGTH; // 设置三个色块的尺寸

    /* 设置混色模式，RGB 是加色模型 */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    /* 绘制红色色块 */
    red.x = (width - red.w) / 2;
    red.y = height / 2 - red.h;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &red);

    /* 绘制绿色色块 */
    green.x = width / 2 - green.w + green.w / 4;
    green.y = height / 3;
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &green);

    /* 绘制蓝色色块 */
    blue.x = width / 2 - blue.w / 4;
    blue.y = green.y;
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &blue);

    return 0;
}
