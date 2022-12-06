#pragma once // 确保只被 include 一次

#include "digitama/big_bang/game.hpp"

namespace WarGrey::STEM {
    class RGBBlock : public WarGrey::STEM::Universe {
        public:
            RGBBlock();
        
        public:
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);
    };
    
    class ContrastColorBlock : public WarGrey::STEM::Universe {
        public:
            ContrastColorBlock();
        
        public:
            void construct(int argc, char* argv[]);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);

        private:
            float hue0;
    };
}
