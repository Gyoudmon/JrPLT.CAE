#include <SDL2/SDL.h>   /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */

#include "line.h"

/*************************************************************************************************/
int draw_dot_and_line(int argc, char* argv[], SDL_Window* window, SDL_Renderer* renderer){
    // 设置窗口标题
    SDL_SetWindowTitle(window, "Draw Dot and Line");

    /** 初始化完成，请开始你的代码 **/
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);   // 设置图形颜色
    SDL_RenderDrawPoint(renderer, 400, 100);                    // 画点

    /** 绘制三角形 **/
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);           // 设置红色
    SDL_RenderDrawLine(renderer, 400, 200, 200, 400);           // 画红色线段
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);           // 设置绿色
    SDL_RenderDrawLine(renderer, 200, 400, 600, 400);           // 画绿色线段
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 255);    // 设置蓝色
    SDL_RenderDrawLine(renderer, 600, 400, 400, 200);           // 画蓝色线段

    return 0;
}
