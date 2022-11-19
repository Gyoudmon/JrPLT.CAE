#pragma once // 确保只被 include 一次

#include "digitama/game.hpp"
#include "digitama/cosmos.hpp"

#include "digitama/graphlet/shapelet.hpp"

namespace WarGrey::STEM {
    /**********************************************************************************************/
    /** 声明游戏世界 **/
    class PaddleBallGame : public WarGrey::STEM::Planet {
        public:
            PaddleBallGame() : Planet("Paddle Ball") {}

        public:    // 覆盖游戏基本方法
            void load(float width, float height) override;
            void reflow(float width, float height) override;
            void update(uint32_t interval, uint32_t count, uint32_t uptime) override;

        protected: // 覆盖键盘事件处理方法
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override;

        private:   // 游戏世界中的物体
            WarGrey::STEM::IShapelet* ball;
            WarGrey::STEM::IShapelet* paddle;
    };

    /** 声明游戏宇宙 **/
    class PaddleBallCosmos : public WarGrey::STEM::Cosmos {
        public:
            PaddleBallCosmos() : Cosmos(60, 0x000000U, 0xFFFFFFU) { /* 什么都不做 */ }

        public:
            void construct(int argc, char* argv[]) override {
                this->push_planet(new PaddleBallGame());
            }
    };
}

