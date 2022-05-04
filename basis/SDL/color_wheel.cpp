#include <SDL2/SDL.h>  /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */

#include "colorspace.hpp"
#include "color_wheel.hpp"
#include "flonum.hpp"
#include "mathematics.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
// 色块边长常量
const int BLOCK_LENGTH = 32;
const float RADIUS = 256.0f;

int draw_color_wheel(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    int width, height;
    SDL_Rect hsb; // 色块变量（矩形
    
    
    SDL_SetWindowTitle(window, "Color Wheel");               // 设置标题
    SDL_GetWindowSize(window, &width, &height);              // 获知窗口大小
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD); // 设置混色模式，RGB 是加色模型
    
    hsb.w = hsb.h = BLOCK_LENGTH;

    for (float hue = 0.0F; hue < 360.0F; hue += 10.0F) {
        HSV_SetRenderDrawColor(renderer, hue, 1.0F, 1.0F, 1.0F);
        
        hsb.x = (width - BLOCK_LENGTH) / 2.0f + RADIUS * flcos(degrees_to_radians(hue - 90.0F));
        hsb.y = (height - BLOCK_LENGTH) / 2.0f + RADIUS * flsin(degrees_to_radians(hue - 90.0F));
        
        SDL_RenderFillRect(renderer, &hsb);
    }

    return 0;
}

