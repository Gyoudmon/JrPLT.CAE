#include "rgb_blocks.hpp"
#include "colorspace.hpp"

/*************************************************************************************************/
// 色块边长常量
const int BLOCK_LENGTH = 200;

// 色块变量（矩形）
SDL_Rect red, green, blue;
SDL_Rect b1, b2, b3;

/*************************************************************************************************/
WarGrey::STEM::RGBBlock::RGBBlock(int width, int height)
    : DrawingBoard("RGB Blocks", width, height) {}

void WarGrey::STEM::RGBBlock::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    // 设置三个色块的尺寸
    red.w = red.h = green.w = green.h = blue.w = blue.h = BLOCK_LENGTH;

    /* 设置混色模式，RGB 是加色模型 */
    this->set_blend_mode(SDL_BLENDMODE_ADD);

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
}

/*************************************************************************************************/
WarGrey::STEM::ContrastColorBlock::ContrastColorBlock(int width, int height)
    : DrawingBoard("Contrast Color Blocks", width, height), hue0(0.0f) {}

void WarGrey::STEM::ContrastColorBlock::construct(int argc, char* argv[]) {
    if (argc > 1) {
        this->hue0 = float(std::atof(argv[1]));
    }

    this->set_window_title("Contrast Color Blocks [hue = %.2f]", this->hue0);

    /* 设置混色模式，RGB 是加色模型 */
    this->set_blend_mode(SDL_BLENDMODE_ADD);
    
    // 设置三个色块的尺寸
    b1.w = b1.h = b2.w = b2.h = b3.w = b3.h = BLOCK_LENGTH;
}

void WarGrey::STEM::ContrastColorBlock::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    /* 绘制红色色块 */
    b1.x = (width - b1.w) / 2;
    b1.y = height / 2 - b1.h;
    HSV_SetRenderDrawColor(renderer, this->hue0 + 0.0F, 1.0F, 1.0F, 1.0F);
    SDL_RenderFillRect(renderer, &b1);

    /* 绘制绿色色块 */
    b2.x = width / 2 - b2.w + b2.w / 4;
    b2.y = height / 3;
    HSV_SetRenderDrawColor(renderer, this->hue0 + 120.0F, 1.0F, 1.0F, 1.0F);
    SDL_RenderFillRect(renderer, &b2);

    /* 绘制蓝色色块 */
    b3.x = width / 2 - b3.w / 4;
    b3.y = b2.y;
    HSV_SetRenderDrawColor(renderer, this->hue0 + 240.0F, 1.0F, 1.0F, 1.0F);
    SDL_RenderFillRect(renderer, &b3);
}

