#pragma once // 确保只被 include 一次

#include "digitama/game.hpp"

namespace WarGrey::STEM {
    /**********************************************************************************************/
    /** 声明游戏宇宙里的物体 **/
    
    /* 定义一个类型，并命名为 Ball（球） */
    struct Ball {
        /* 球的位置 */
        int x;
        int y;
        
        /* 球位置的变化量 */
        int dx;
        int dy;
    };

    /* 定义一个类型，并命名为 Paddle（桨） */
    class Paddle {
        public:
            /* 桨的位置 */
            int x;
            int y;
    };

    /**********************************************************************************************/
    /** 声明游戏宇宙 **/
    class PaddleBallGame : public WarGrey::STEM::Universe {
        public:
            PaddleBallGame();
            virtual ~PaddleBallGame();

        public:    // 覆盖游戏基本方法
            void reflow(int width, int height);
            void update(uint32_t interval, uint32_t count, uint32_t uptime);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);

        protected: // 覆盖输入事件处理方法
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed);  // 处理键盘事件

        private:
            WarGrey::STEM::Ball ball;
            WarGrey::STEM::Paddle paddle;

        private:
            int screen_width;
            int screen_height;
    };
}

