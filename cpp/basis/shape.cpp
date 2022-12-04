// shape.cpp 文件
#include "shape.hpp"  // 导入本模块自己的头文件

using namespace WarGrey::STEM; // 声明本模块的类和函数默认以 WarGrey::STEM 的名义使用

/*************************************************************************************************/
// 实现 ShapeWorld::construct 方法，本例中仅设置窗口大小
void WarGrey::STEM::ShapeWorld::construct(int argc, char* argv[]) {
    this->set_window_size(800, 600);
}

// 实现 ShapeWorld::load 方法，在舞台上加入基础几何图形的实例，注意添加顺序
void WarGrey::STEM::ShapeWorld::load(float width, float height) {
    this->label = this->insert(new Labellet(game_font::unicode, 0U, "基本图形陈列馆(对象版)"));

    // 苍绿色院子
    this->garden = this->insert(new Ellipselet(200, 80, PALEGREEN, KHAKI));
    
    // 房屋的组成
    this->roof = this->insert(new RegularPolygonlet(3, 140.0F, -90.0, DEEPSKYBLUE, ROYALBLUE)); // 深空蓝屋顶
    this->wall = this->insert(new Rectanglet(200, 180, WHITESMOKE, SNOW));                      // 白色墙壁
    this->door = this->insert(new Rectanglet(42, 84, KHAKI, DARKKHAKI));                        // 卡其色门
    this->lock = this->insert(new Circlet(4, CHOCOLATE));                                       // 巧克力色门锁
    this->window = this->insert(new RoundedSquarelet(64, -0.15F, LIGHTSKYBLUE, SKYBLUE));       // 天蓝色窗户
}

// 实现 ShapeWorld::reflow 方法，重新排列几何图形在舞台上的位置
void WarGrey::STEM::ShapeWorld::reflow(float width, float height) {
    // 排列基本图形以组装房屋
    this->move_to(this->roof, width * 0.5F, height * 0.64F, MatterAnchor::CB);
    this->move_to(this->wall, this->roof, MatterAnchor::CB, MatterAnchor::CT, 0.0F, -70.0F);
    this->move_to(this->door, this->wall, MatterAnchor::LB, MatterAnchor::LB, 24.0F);
    this->move_to(this->lock, this->door, MatterAnchor::RC, MatterAnchor::RC, -4.0F);
    this->move_to(this->window, this->wall, MatterAnchor::CC, MatterAnchor::LC);

    // 排列院子
    this->move_to(this->garden, this->wall, MatterAnchor::CC, MatterAnchor::CT);
}