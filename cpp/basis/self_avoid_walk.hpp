#ifndef _ANIMATED_SELF_AVOID_WALK_H // 确保只被 include 一次
#define _ANIMATED_SELF_AVOID_WALK_H

#include "digitama/game.hpp"

namespace WarGrey::STEM {
#define WORLD_SIZE 48    // 方格单边数量

    enum class WalkState { Run, Succeed, Failed };

    class SelfAvoidWalk : public WarGrey::STEM::Universe {
        public:
            SelfAvoidWalk();
        
        public:
            void construct(int argc, char* argv[]);
            void reflow(int width, int height);
            void update(uint32_t interval, uint32_t count, uint32_t uptime);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);

        private:
            int world_x;
            int world_y;
            int lineheight;

        private:
            SDL_Rect grid_self;
            int grids[WORLD_SIZE + 2][WORLD_SIZE + 2];
            int x;
            int y;
            WarGrey::STEM::WalkState state;
    };
}

#endif

