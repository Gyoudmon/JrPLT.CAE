#include <SDL2/SDL.h>  /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */

#include "colorspace.h"
#include "rainbow.h"
#include "flonum.h"
#include "mathematics.h"

using namespace WarGrey::STEM;

/*************************************************************************************************/
// 色块边长常量
const int BLOCK_LENGTH = 20;
const int RADIUS = 200;

int draw_rainbow(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    int width, height;
    SDL_Rect hsb; // 色块变量（矩形）
    
    
    SDL_SetWindowTitle(window, "Rainbow");                   // 设置标题
    SDL_GetWindowSize(window, &width, &height);              // 获知窗口大小
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD); // 设置混色模式，RGB 是加色模型
    
    hsb.w = hsb.h = BLOCK_LENGTH;

    for (double hue = 0.0; hue < 360.0; hue += 10.0) {
        HSV_SetRenderDrawColor(renderer, hue, 1.0, 1.0, 1.0);
        
        hsb.x = (width - BLOCK_LENGTH) / 2 + RADIUS * flcos(degrees_to_radians(hue));
        hsb.y = (height - BLOCK_LENGTH) / 2 + RADIUS * flsin(degrees_to_radians(hue));
        
        SDL_RenderFillRect(renderer, &hsb);
    }

    return 0;
}
