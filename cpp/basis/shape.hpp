// shape.hpp 文件
#pragma once // 确保只被 include 一次

#include "digitama/game.hpp" // 导入游戏模块，包含自己项目里的头文件要用双引号

namespace WarGrey::STEM /* 以 WarGrey::STEM 的名义提供工具 */ {
    // 创建自定义数据类型，并命名为 PrimitiveShape，继承自 Pasteboard
    class PrimitiveShape : public WarGrey::STEM::Pasteboard {
        public:
            PrimitiveShape(); // 构造函数
        
        public:
            // 创建游戏世界，充当真正的 main 函数
            void construct(int argc, char* argv[]);
            
             // 绘制游戏世界，将游戏内容呈现在屏幕上
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height); 
    };
}

