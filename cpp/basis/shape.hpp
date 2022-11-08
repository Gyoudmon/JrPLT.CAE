#pragma once // 确保只被 include 一次

#include "digitama/game.hpp"

namespace WarGrey::STEM {
    class PrimitiveShape : public WarGrey::STEM::Pasteboard {
        public:
            PrimitiveShape();
        
        public:
            void construct(int argc, char* argv[]);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);
    };
}

