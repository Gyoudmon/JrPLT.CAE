// shape.hpp 文件
#pragma once // 确保只被 include 一次

#include "digitama/game.hpp"

#include "digitama/matter/graphlet/shapelet.hpp"   // 导入基本图形模块
#include "digitama/matter/graphlet/ui/textlet.hpp" // 导入UI文本模块

namespace WarGrey::STEM /* 以 WarGrey::STEM 的名义提供工具 */ {
    // 创建自定义数据类型，并命名为 ShapeWorld，继承自 World
    class ShapeWorld : public WarGrey::STEM::World {
        public:
            // 构造函数，默认什么都不做，除了通过父类构造函数设置窗口标题和帧频
            ShapeWorld() : World("Primitive Shapes", 0) {}

            // 析构函数，默认什么都不做
            virtual ~ShapeWorld() {}
        
        public:
            // 创建游戏世界，充当真正的 main 函数
            void construct(int argc, char* argv[]) override;
            
            // 加载游戏中的物体(包括前景、背景、建筑、游戏角色等等)
            void load(float width, float height) override;

            // 布局游戏对象，让它们待在正确的位置上
            void reflow(float width, float height) override;

        public: // 为演示该设计思路的优点，运行游戏里的物体可以被选中
            bool can_select(WarGrey::STEM::IMatter* g) override { return true; }

        private: // 本游戏世界中的有以下物体
            WarGrey::STEM::Labellet* label;
            WarGrey::STEM::IShapelet* lines[3];
            WarGrey::STEM::IShapelet* rectangle;
            WarGrey::STEM::IShapelet* circle;
            WarGrey::STEM::IShapelet* ellipse;
            WarGrey::STEM::IShapelet* regular_polygons[10];
    };
}
