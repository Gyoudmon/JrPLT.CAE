#ifndef _PADDLE_BALL_H // 确保只被 include 一次
#define _PADDLE_BALL_H

#include "game.hpp"

namespace WarGrey::STEM {
    /** 声明游戏宇宙 **/
    class PaddleBall : public WarGrey::STEM::Universe {
        public:
            PaddleBall();
            virtual ~PaddleBall();

        public:    // 覆盖游戏基本方法
            void reflow(int width, int height);
            void update(uint32_t interval, uint32_t count, uint32_t uptime);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);

        protected: // 覆盖输入事件处理方法
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed);  // 处理键盘事件

        private:
            int ball_x;
            int ball_y;
            int paddle_x;
            int paddle_y;

        private:
            int ball_dx;
            int ball_dy;

        private:
            int screen_width;
            int screen_height;
    };
}

#endif

