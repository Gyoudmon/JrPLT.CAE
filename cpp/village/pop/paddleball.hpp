#pragma once // 确保只被 include 一次

#include "../../digitama/game.hpp"

namespace WarGrey::STEM {
    /************************************** 声明游戏宇宙里的物体 ***************************************/
    /* 定义一个类型，并命名为 Ball（球） */
    struct Ball {
        /* 球的位置 */
        float x = 0.0F;
        float y = 0.0F;
        
        /* 球位置的变化量 */
        float dx = 0.0F;
        float dy = 0.0F;

        /* 球的颜色 */
        uint32_t color = ORANGE;
    };

    /* 定义一个类型，并命名为 Paddle（桨） */
    class Paddle {
        public: /* <- class 定义的类型，其属性和方法默认为 private，必须明确指明为 public  */
            /* 桨的位置 */
            float x = 0.0F;
            float y = 0.0F;

            /* 桨的速度 */
            float speed = 0.0F;
    };

    /******************************************* 声明游戏世界 ******************************************/
    class PaddleBallWorld : public WarGrey::STEM::World {
        public:
            PaddleBallWorld() : World("Paddle Ball (POP)") { /* 什么都不做 */ }
            virtual ~PaddleBallWorld() { /* 什么都不做 */ }

        public:    // 覆盖游戏基本方法
            void reflow(float width, float height) override;
            void update(uint32_t interval, uint32_t count, uint32_t uptime) override;
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height) override;

        protected: // 覆盖键盘事件处理方法
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override;

        private:   // 本游戏世界中的物体
            WarGrey::STEM::Ball ball;
            WarGrey::STEM::Paddle paddle;
    };
}
