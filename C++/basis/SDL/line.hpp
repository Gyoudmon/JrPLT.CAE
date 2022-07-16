#ifndef _DOT_AND_LINE_H // 确保只被 include 一次
#define _DOT_AND_LINE_H

#include "game.hpp"

namespace WarGrey::STEM {
    class DotAndLine : public WarGrey::STEM::DrawingBoard {
        public:
            DotAndLine(int width, int height);
        
        public:
            void draw(SDL_Renderer* renderer, int x, int y, int width, int height);
    };
}

#endif

