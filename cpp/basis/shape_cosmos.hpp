// shape.hpp 文件
#pragma once // 确保只被 include 一次

#include "digitama/game.hpp"   // 导入游戏模块，包含自己项目里的头文件要用双引号
#include "digitama/cosmos.hpp" // 导入 Cosmos 模块

#include "digitama/graphlet/shapelet.hpp"   // 导入基本图形模块
#include "digitama/graphlet/ui/textlet.hpp" // 导入UI文本模块

namespace WarGrey::STEM /* 以 WarGrey::STEM 的名义提供工具 */ {
    // 创建自定义数据类型，并命名为 ShapePlanet，继承自 Planet，代表游戏世界
    class ShapePlanet : public WarGrey::STEM::Planet {
        public:
            ShapePlanet() : Planet("Primitive Shape Cosmos") {}

        public:    // 覆盖游戏基本方法
            void load(float width, float height) override;
            void reflow(float width, float height) override;

        private:   // 游戏世界中的物体
            WarGrey::STEM::Labellet* label;
            WarGrey::STEM::IShapelet* lines[3];
            WarGrey::STEM::IShapelet* rectangle;
            WarGrey::STEM::IShapelet* circle;
            WarGrey::STEM::IShapelet* ellipse;
            WarGrey::STEM::IShapelet* regular_polygons[10];
    };

    /*********************************************************************************************/
    // 创建自定义数据类型，并命名为 ShapeCosmos，继承自 Cosmos，代表游戏宇宙
    class ShapeCosmos : public WarGrey::STEM::Cosmos {
        public:
            // 构造函数，默认什么都不做
            // 除了通过父类构造函数设置帧频为0，即非动画宇宙
            ShapeCosmos() : Cosmos(0) {}
        
        public:
            // 创建游戏宇宙，加入游戏世界 PrimitiveShapePlanet
            void construct(int argc, char* argv[]) {
                this->set_window_size(1200, 800);
                this->push_planet(new ShapePlanet());
            }
    };
}

