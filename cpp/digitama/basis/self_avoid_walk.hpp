#pragma once // 确保只被 include 一次

#include "../big_bang/bang.hpp"

namespace WarGrey::STEM {
#define GRID_SIZE 13    // 方格单边数量

    class SelfAvoidWalkWorld : public WarGrey::STEM::TheBigBang {
        public:
            SelfAvoidWalkWorld() : TheBigBang("自回避随机漫步") {}
        
        public:
            void load(float width, float height) override;
            void reflow(float width, float height) override;
            void update(uint64_t count, uint32_t interval, uint64_t uptime) override;
            void on_mission_start(float width, float height) override;

        public:
            bool can_select(WarGrey::STEM::IMatter* m) override;

        protected:
            void after_select(WarGrey::STEM::IMatter* m, bool yes) override;
            bool update_tooltip(WarGrey::STEM::IMatter* m, float lx, float ly, float gx, float gy) override;

        protected:
            void reset_walkers(bool keep_mode);
            void reset_maze();
            
        private:
            float world_x;
            float world_y;
            float grid_width = 0.0F;
            float grid_height = 0.0F;

        private:
            WarGrey::STEM::PlanetCuteTile* grounds[GRID_SIZE + 2][GRID_SIZE + 2];
            WarGrey::STEM::Bracer* walkers[8];
            int grids[GRID_SIZE + 2][GRID_SIZE + 2];

        private:
            WarGrey::STEM::Bracer* current_walker = nullptr;
            int x = 0;
            int y = 0;
    };
}
