#ifndef _ANIMATED_DIFFUSION_H // 确保只被 include 一次
#define _ANIMATED_DIFFUSION_H

#include "game.hpp"

namespace WarGrey::STEM {
#define PARTICLE_COUNT 128
    
    class Diffusion : public WarGrey::STEM::DrawingPlayer {
        public:
            Diffusion(int width, int height);
        
        public:
            void construct(int argc, char* argv[]);
            void update(uint32_t interval, uint32_t count, uint32_t uptime);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);

        private: // 声明静态数组
            int xs[PARTICLE_COUNT];
            int ys[PARTICLE_COUNT];

        private:
            int ceiling;
    };
}

#endif

