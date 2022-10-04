#ifndef _PRIMITIVE_SHAPE_H // 确保只被 include 一次
#define _PRIMITIVE_SHAPE_H

#include "game.hpp"

namespace WarGrey::STEM {
    class PrimitiveShape : public WarGrey::STEM::DrawingBoard {
        public:
            PrimitiveShape();
        
        public:
            void construct(int argc, char* argv[]);
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);
    };
}

#endif


