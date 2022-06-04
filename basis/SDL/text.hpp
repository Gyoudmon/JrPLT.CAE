#ifndef _WARGREY_TEXT_H
#define _WARGREY_TEXT_H

#include <SDL2/SDL_ttf.h>

#include <string>
#include <cstdint>

namespace WarGrey::STEM {
    enum TextRenderMode { Solid, Shaded };

    std::string game_create_string(const char* fmt, ...);

    void game_text_size(TTF_Font* font, int* width, int* height, const char* fmt, ...);
    void game_draw_solid_text(TTF_Font* font, SDL_Renderer* renderer, unsigned int rgba, int x, int y, const char* fmt, ...);
    void game_draw_shaded_text(TTF_Font* font, SDL_Renderer* renderer, unsigned int fgc, unsigned int bgc, int x, int y, const char* fmt, ...);
    void game_draw_blended_text(TTF_Font* font, SDL_Renderer* renderer, unsigned int rgba, int x, int y, const char* fmt, ...);
}

#endif

