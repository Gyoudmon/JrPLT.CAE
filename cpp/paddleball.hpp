#pragma once // 确保只被 include 一次

#include "digitama/bang.hpp"

#include "digitama/big_bang/matter/graphlet/shapelet.hpp"

namespace WarGrey::STEM {
    /******************************************* 声明游戏世界 ******************************************/
    class PaddleBallWorld : public WarGrey::STEM::TheBigBang {
        public:
            PaddleBallWorld() : TheBigBang("托球游戏") {}

        public:    // 覆盖游戏基本方法
            void load(float width, float height) override;
            void update(uint32_t interval, uint32_t count, uint32_t uptime) override;

        public:
            void on_mission_start(float width, float height) override;

        protected: // 覆盖键盘事件处理方法
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override;

        private:   // 本游戏世界中的物体
            WarGrey::STEM::IShapelet* ball;
            WarGrey::STEM::IShapelet* paddle;
    };
}
