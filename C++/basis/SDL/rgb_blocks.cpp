#include "rgb_blocks.hpp"
#include "colorspace.hpp"

/*************************************************************************************************/
WarGrey::STEM::RGBBlock::RGBBlock(int width, int height)
    : DrawingBoard("RGB Blocks", width, height) {}

void WarGrey::STEM::RGBBlock::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    int r = 100;

    /* 思考：为什么背景一定要是黑色的？ */
    game_fill_rect(renderer, x, y, width, height, 0x000000U);

    /* 设置混色模式，RGB 属加色模型 */
    this->set_blend_mode(SDL_BLENDMODE_ADD);

    game_fill_circle(renderer, width / 2,       (height - r) / 2, r, 0xFF0000U); // 红色
    game_fill_circle(renderer, (width - r) / 2, (height + r) / 2, r, 0x00FF00U); // 绿色
    game_fill_circle(renderer, (width + r) / 2, (height + r) / 2, r, 0x0000FFU); // 蓝色
}

/*************************************************************************************************/
WarGrey::STEM::ContrastColorBlock::ContrastColorBlock(int width, int height)
    : DrawingBoard("Contrast Color Blocks", width, height, 0xFFFFFFFFU, 0x0000FFU), hue0(0.0f) {}

void WarGrey::STEM::ContrastColorBlock::construct(int argc, char* argv[]) {
    if (argc > 1) {
        this->hue0 = float(std::atof(argv[1]));
    }

    this->set_window_title("Contrast Color Blocks [hue = %.2f]", this->hue0);

    /* 设置混色模式，RGB 是加色模型 */
    this->set_blend_mode(SDL_BLENDMODE_ADD);
}

void WarGrey::STEM::ContrastColorBlock::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    int apothem = 100;

    game_fill_square(renderer, width / 2,             (height - apothem) / 2, apothem, this->hue0 + 000.0f);
    game_fill_square(renderer, (width - apothem) / 2, (height + apothem) / 2, apothem, this->hue0 + 120.0f);
    game_fill_square(renderer, (width + apothem) / 2, (height + apothem) / 2, apothem, this->hue0 + 240.0f);
}

