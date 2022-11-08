#pragma once // 确保只被 include 一次

#include "digitama/game.hpp"

namespace WarGrey::STEM {
    class RGBBlock : public WarGrey::STEM::Pasteboard {
        public:
            RGBBlock();
        
        public:
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);
    };
    
    class ContrastColorBlock : public WarGrey::STEM::Pasteboard {
        public:
            ContrastColorBlock();
        
        public:
            void construct(int argc, char* argv[]);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);

        private:
            float hue0;
    };
}
