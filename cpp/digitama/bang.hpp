#pragma once

#include "big_bang/graphics/font.hpp"
#include "big_bang/physics/random.hpp"

namespace WarGrey::STEM {
    /* Constants */
    static const char* title_fmt = "青少计算机科学: %s";
    
    /* Fonts */
    static const int title_fontsize = 42; /* <-- already hit the ceiling */
    static const int normal_fontsize = 24;
    static const int tiny_fontsize = 20;

    class bang_font {
    public:
        static TTF_Font* title;
        static TTF_Font* normal;
        static TTF_Font* tiny;
        static TTF_Font* vertical;
        static TTF_Font* mono;
    };

    void bang_fonts_initialize();
    void bang_fonts_destroy();
}
