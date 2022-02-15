#include <SDL2/SDL.h>  /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */

#include "colorspace.h"
#include "rainbow.h"
#include "flonum.h"
#include "mathematics.h"

using namespace WarGrey::STEM;

/*************************************************************************************************/
// 彩虹常量
const float RAINBOW_RADIUS = 128.0F;
const float RAINBOW_WIDTH = 256.0F;

// 取样参数
const float HUE_DELTA = 1.0F;
const float SAMPLE_DELTA = 0.1F;

int draw_rainbow(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    int width, height;
    float x, y;
    
    SDL_SetWindowTitle(window, "Rainbow");                    // 设置标题
    SDL_GetWindowSize(window, &width, &height);               // 获知窗口大小
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); // 设置混色模式, 不要混色！！！
    
    for (float hue = 0.0f; hue < 360.0f; hue += HUE_DELTA) {
        for (float theta = 0.0f; theta < 360.0f; theta += SAMPLE_DELTA) {
            float r = (360.0F - hue) / 360.0F * RAINBOW_WIDTH + RAINBOW_RADIUS;

            HSV_SetRenderDrawColor(renderer, hue - 75.0F, 1.0F, 1.0F, 1.0F);
        
            x = width / 2.0f + r * flcos(degrees_to_radians(theta));
            y = height / 2.0f + r * flsin(degrees_to_radians(theta));
        
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    return 0;
}

