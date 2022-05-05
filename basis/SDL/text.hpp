#ifndef _WARGREY_TEXT_H
#define _WARGREY_TEXT_H

#include <SDL2/SDL_ttf.h>

#include <string>
#include <cstdint>

namespace WarGrey::STEM {
    std::string game_create_string(const char* fmt, ...);

    void game_text_size(TTF_Font* font, int* width, int* height, const char* fmt, ...);
}

#endif

