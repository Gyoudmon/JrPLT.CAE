#pragma once // 确保只被 include 一次

#include "digitama/world.hpp"

namespace WarGrey::STEM {
    /**********************************************************************************************/
    /** 声明游戏宇宙里的物体 **/
    
    /* 定义一个类型，并命名为 Ball（球） */
    struct Ball {
        /* 球的位置 */
        int x = 0;
        int y = 0;
        
        /* 球位置的变化量 */
        int dx = 0;
        int dy = 0;
    };

    /* 定义一个类型，并命名为 Paddle（桨） */
    class Paddle {
        public:
            /* 桨的位置 */
            int x = 0;
            int y = 0;

        public:
            /* 桨的速度 */
            int speed = 0;
    };

    /**********************************************************************************************/
    /** 声明游戏宇宙 **/
    class PaddleBall : public WarGrey::STEM::World {
        public:
            PaddleBall() : World("Paddle Ball") { /* 什么都不做 */ }
            virtual ~PaddleBall() { /* 什么都不做 */ }

        public:    // 覆盖游戏基本方法
            void reflow(float width, float height) override;
            void update(uint32_t interval, uint32_t count, uint32_t uptime) override;
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height) override;

        protected: // 覆盖键盘事件处理方法
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override;

        private:   // 游戏宇宙中的物体
            WarGrey::STEM::Ball ball;
            WarGrey::STEM::Paddle paddle;

        private:
            int screen_width;
            int screen_height;
    };
}

