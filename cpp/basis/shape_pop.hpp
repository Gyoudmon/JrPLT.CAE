// shape.hpp 文件
#pragma once // 确保只被 include 一次

// 导入游戏模块，内含 World 类和常用函数，包含自己项目里的头文件要用双引号
#include "digitama/game.hpp"

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
            void construct(int argc, char* argv[]);
            
            // 绘制游戏世界，将游戏内容呈现在屏幕上
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height); 
    };
}
