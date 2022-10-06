#ifndef _RANDOM_WALK_H // 确保只被 include 一次
#define _RANDOM_WALK_H

#include "digitama/game.hpp"

namespace WarGrey::STEM {
    class RandomWalk : public WarGrey::STEM::Universe {
        public:
            RandomWalk(const char* title = "Random Walk");
        
        public:
            void reflow(int width, int height);
            void update(uint32_t interval, uint32_t count, uint32_t uptime);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);

        protected:
            int x;
            int y;
    };
    
    class DrunkardWalk : public WarGrey::STEM::RandomWalk {
        public:
            DrunkardWalk();
        
        public:
            void update(uint32_t interval, uint32_t count, uint32_t uptime);
    };
}

#endif

