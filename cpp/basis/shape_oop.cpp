// shape.cpp 文件
#include "shape_oop.hpp"  // 导入本模块自己的头文件

using namespace WarGrey::STEM; // 声明本模块的类和函数默认以 WarGrey::STEM 的名义使用

/*************************************************************************************************/
// 实现 ShapeWorld::construct 方法，本例中仅设置窗口大小
void WarGrey::STEM::ShapeWorld::construct(int argc, char* argv[]) {
    this->set_window_size(1200, 800);
}

// 实现 ShapeWorld::load 方法，在舞台上加入基础几何图形的实例
void WarGrey::STEM::ShapeWorld::load(float width, float height) {
    this->label = this->insert(new Labellet(game_font::unicode, 0U, "基本图形陈列馆(对象版)"));

    // 三角形的三条边
    this->lines[0] = this->insert(new Linelet(-200, +200, RED));
    this->lines[1] = this->insert(new Linelet(+400, +000, GREEN));
    this->lines[2] = this->insert(new Linelet(+200, +200, BLUE));

    this->rectangle = this->insert(new Rectanglet(200, 100, YELLOWGREEN, ORANGE));  // 橘边黄绿色长方形
    this->circle = this->insert(new Circlet(50, PINK, PURPLE));                     // 紫边粉红色圆
    this->ellipse = this->insert(new Ellipselet(64, 32, KHAKI, SKYBLUE));           // 天蓝色边卡其色椭圆
    
    for (int n = 1; n <= sizeof(this->regular_polygons) / sizeof(IShapelet*); n++) {
        this->regular_polygons[n - 1] = this->insert(new RegularPolygonlet(n, 32, -90.0, ROYALBLUE, FIREBRICK));
    }
}

// 实现 ShapeWorld::reflow 方法，重新排列几何图形在舞台上的位置
void WarGrey::STEM::ShapeWorld::reflow(float width, float height) {
    // 排列线段以组成三角形
    this->move_to(this->lines[0], 200.0F, 400.0F, MatterAnchor::LB); // 左下角对齐
    this->move_to(this->lines[1], 200.0F, 400.0F, MatterAnchor::LT); // 左上角对齐
    this->move_to(this->lines[2], 600.0F, 400.0F, MatterAnchor::RB); // 右下角对齐

    // 排列长方形和椭圆
    this->move_to(this->rectangle, 800.0F, 100.0F, MatterAnchor::LT);
    this->move_to(this->circle, 900.0F, 400.0F, MatterAnchor::CC);
    this->move_to(this->ellipse, 900.0F, 600.0F, MatterAnchor::CC);

    // 排列正多边形
    for (int i = 0; i < sizeof(this->regular_polygons) / sizeof(IShapelet*); i++) {
        this->move_to(this->regular_polygons[i], 100.0F * float(i), 750.0F, MatterAnchor::CC);
    }
}

