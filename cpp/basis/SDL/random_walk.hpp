#ifndef _RANDOM_WALK_H // 确保只被 include 一次
#define _RANDOM_WALK_H

#include "game.hpp"

namespace WarGrey::STEM {
    class RandomWalk : public WarGrey::STEM::DrawingPlayer {
        public:
            RandomWalk(int width, int height, const char* title = "Random Walk");
        
        public:
            void update(uint32_t interval, uint32_t count, uint32_t uptime);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);

        protected:
            int x;
            int y;
    };
    
    class DrunkardWalk : public WarGrey::STEM::RandomWalk {
        public:
            DrunkardWalk(int width, int height);
        
        public:
            void update(uint32_t interval, uint32_t count, uint32_t uptime);
    };
}

#endif

