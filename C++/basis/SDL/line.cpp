#include "line.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
WarGrey::STEM::DotAndLine::DotAndLine(int width, int height) : DrawingBoard("Dot and Line", width, height) {}

void WarGrey::STEM::DotAndLine::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_RenderDrawPoint(renderer, 400, 100);                    // 画点

    /** 绘制三角形 **/
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);           // 设置红色
    SDL_RenderDrawLine(renderer, 400, 200, 200, 400);           // 画红色线段
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);           // 设置绿色
    SDL_RenderDrawLine(renderer, 200, 400, 600, 400);           // 画绿色线段
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 255);    // 设置蓝色
    SDL_RenderDrawLine(renderer, 600, 400, 400, 200);           // 画蓝色线段
}

