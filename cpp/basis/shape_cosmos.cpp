// shape.cpp 文件
#include "shape_cosmos.hpp"  // 导入自己的头文件

using namespace WarGrey::STEM; // 声明本模块的类和函数默认以 WarGrey::STEM 的名义使用

/*************************************************************************************************/
void WarGrey::STEM::ShapeCosmos::construct(int argc, char* argv[]) {
    this->set_window_size(1200, 800);
}

// 实现 ShapeCosmos::load 方法，在舞台上加入基础几何图形的实例
void WarGrey::STEM::ShapeCosmos::load(float width, float height) {
    this->label = self->insert_one(new Labellet(game_font::unicode, 0U, "基本图形陈列馆"));

    this->lines[0] = self->insert_one(new Linelet(-200, +200, RED));
    this->lines[1] = self->insert_one(new Linelet(+400, +000, GREEN));
    this->lines[2] = self->insert_one(new Linelet(+200, +200, BLUE));

    this->rectangle = self->insert_one(new Rectanglet(200, 100, YELLOWGREEN, ORANGE));  // 橘边黄绿色长方形
    this->circle = self->insert_one(new Circlet(50, PINK, PURPLE));                   // 紫边粉红色圆
    this->ellipse = self->insert_one(new Ellipselet(64, 32, KHAKI, SKYBLUE));           // 天蓝色边卡其色椭圆
    
    for (int n = 1; n <= sizeof(this->regular_polygons) / sizeof(IShapelet*); n++) {
        this->regular_polygons[n - 1] = self->insert_one(new RegularPolygonlet(n, 32, -90.0, ROYALBLUE, FIREBRICK));
    }
}

// 实现 ShapeCosmos::reflow 方法，从新排列几何图形在舞台上的位置
void WarGrey::STEM::ShapeCosmos::reflow(float width, float height) {
    // 排列以组合三角形
    self->move_to(this->lines[0], 200, 400, MatterAnchor::LB); // 左下角对齐
    self->move_to(this->lines[1], 200, 400, MatterAnchor::LT); // 左上角对齐
    self->move_to(this->lines[2], 600, 400, MatterAnchor::RB); // 右下角对齐

    // 排列长方形和椭圆
    self->move_to(this->rectangle, 800, 100, MatterAnchor::LT);
    self->move_to(this->circle, 900, 400, MatterAnchor::CC);
    self->move_to(this->ellipse, 900, 600, MatterAnchor::CC);

    // 排列正多边形
    for (int i = 0; i < sizeof(this->regular_polygons) / sizeof(IShapelet*); i++) {
        self->move_to(this->regular_polygons[i], 100 * i, 750, MatterAnchor::CC);
    }
}

