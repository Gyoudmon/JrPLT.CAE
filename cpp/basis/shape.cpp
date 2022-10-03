#include "shape.hpp"

#include "text.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
WarGrey::STEM::PrimitiveShape::PrimitiveShape(int width, int height)
    : DrawingBoard("Primitive Shapes", width, height) {}

void WarGrey::STEM::PrimitiveShape::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    game_draw_solid_text(game_unicode_font, renderer,
            this->get_foreground_color(), 0, 0, "基本图形");

    game_draw_point(renderer, 600, 300, ROYALBLUE);             // 画皇家蓝点

    /** 绘制三角形 **/
    game_draw_line(renderer, 400, 200, 200, 400, RED);          // 画红色线段
    game_draw_line(renderer, 200, 400, 600, 400, GREEN);        // 画绿色线段
    game_draw_line(renderer, 600, 400, 400, 200, BLUE);         // 画蓝色线段

    /** 绘制矩形 **/
    game_fill_rect(renderer, 800, 100, 200, 100, YELLOWGREEN);  // 画黄绿色矩形
    game_draw_rect(renderer, 800, 100, 200, 100, ORANGE);       // 画橘色边框

    /** 绘制圆 **/
    game_fill_circle(renderer, 900, 400, 50, PINK);             // 画粉红色圆
    game_draw_circle(renderer, 900, 400, 50, PURPLE);           // 画紫色轮廓

    /** 绘制椭圆 **/
    game_fill_ellipse(renderer, 900, 600, 64, 32, KHAKI);       // 画卡其色椭圆
    game_draw_ellipse(renderer, 900, 600, 64, 32, SKYBLUE);     // 画天蓝色轮廓
}


