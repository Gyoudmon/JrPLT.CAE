// shape.cpp 文件
#include "shape_universe.hpp" // 导入自己的头文件

#include "digitama/text.hpp" // 还用到了“画文字”函数

using namespace WarGrey::STEM; // 声明本模块的类和函数默认以 WarGrey::STEM 的名义使用

/*************************************************************************************************/
// 实现 ShapeUniverse::construct 方法，设置窗口大小
void WarGrey::STEM::ShapeUniverse::construct(int argc, char* argv[]) {
    this->set_window_size(1200, 800);
}

// 实现 ShapeUniverse::draw 方法，本例中绘制一系列几何图形
void WarGrey::STEM::ShapeUniverse::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    game_draw_blended_text(game_font::unicode, renderer,
            this->get_foreground_color(), 10, 10, "基本图形陈列馆");

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

    /** 绘制正多边形 **/
    for (int n = 1; n < 11; n++) {
        game_fill_regular_polygon(renderer, n, 100 * (n - 1), 750, 32, -90.0, ROYALBLUE);
        game_draw_regular_polygon(renderer, n, 100 * (n - 1), 750, 32, -90.0, FIREBRICK);
    }
}

