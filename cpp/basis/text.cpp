#include "game.hpp"                 // 放最前面以兼容 macOS
#include "text.hpp"
#include "vsntext.hpp"
#include "colorspace.hpp"

#include <cstdarg>
#include <iostream>

using namespace WarGrey::STEM;

/*************************************************************************************************/
enum TextRenderMode { SOLID, SHADED, BLENDED };

/*************************************************************************************************/
static inline void unsafe_utf8_size(TTF_Font* font, int* width, int* height, const std::string& text) {
    if (TTF_SizeUTF8(font, text.c_str(), width, height)) {
        fprintf(stderr, "无法计算文本尺寸: %s\n", TTF_GetError());
    }
}

static inline void setup_for_text(TTF_Font* font, const std::string& text, unsigned int rgb, SDL_Color& c) {
    RGB_FromHexadecimal(rgb, &c.r, &c.g, &c.b);
    c.a = 0xFFU;
}

static SDL_Surface* game_text_surface(const std::string& text, ::TextRenderMode mode, TTF_Font* font, SDL_Color& fgc, SDL_Color& bgc) {
    SDL_Surface* surface = NULL;

    switch (mode) {
        case ::TextRenderMode::SOLID: surface = TTF_RenderUTF8_Solid(font, text.c_str(), fgc); break;
        case ::TextRenderMode::SHADED: surface = TTF_RenderUTF8_Shaded(font, text.c_str(), fgc, bgc); break;
        case ::TextRenderMode::BLENDED: surface = TTF_RenderUTF8_Blended(font, text.c_str(), fgc); break;
    }

    if (surface == NULL) {
        fprintf(stderr, "无法渲染文本: %s with %s\n", TTF_GetError(), TTF_FontFaceStyleName(font));
    }

    return surface;
}

static inline void safe_render_text_surface(SDL_Renderer* target, SDL_Surface* message, int x, int y) {
    /** TODO: Cache the textures of text **/

    if (message != NULL) {
        game_render_surface(target, message, x, y);
        SDL_FreeSurface(message);
    }
}

/*************************************************************************************************/
std::string WarGrey::STEM::game_create_string(const char* fmt, ...) {
    VSNPRINT(s, fmt);

    return s;
}

/*************************************************************************************************/
void WarGrey::STEM::game_text_size(TTF_Font* font, int* width, int* height, const char* fmt, ...) {
    VSNPRINT(text, fmt);

    if (font == NULL) {
        font = GAME_DEFAULT_FONT;
    }

    unsafe_utf8_size(font, width, height, text);
}
    
void WarGrey::STEM::game_draw_solid_text(TTF_Font* font, SDL_Renderer* renderer, uint32_t rgb, int x, int y, const char* fmt, ...) {
    SDL_Color text_color;

    VSNPRINT(text, fmt);

    if (font == NULL) {
        font = GAME_DEFAULT_FONT;
    }

    setup_for_text(font, text, rgb, text_color);
    SDL_Surface* message = game_text_surface(text, ::TextRenderMode::SOLID, font, text_color, text_color);
    safe_render_text_surface(renderer, message, x, y);
}

void WarGrey::STEM::game_draw_shaded_text(TTF_Font* font, SDL_Renderer* renderer, uint32_t fgc, uint32_t bgc, int x, int y, const char* fmt, ...) {
    SDL_Color text_color, background_color;

    VSNPRINT(text, fmt);

    if (font == NULL) {
        font = GAME_DEFAULT_FONT;
    }

    setup_for_text(font, text, fgc, text_color);
    RGB_FromHexadecimal(bgc, &background_color.r, &background_color.g, &background_color.b);
    background_color.a = 0xFF;
    SDL_Surface* message = game_text_surface(text, ::TextRenderMode::SHADED, font, text_color, background_color);
    safe_render_text_surface(renderer, message, x, y);
}

void WarGrey::STEM::game_draw_blended_text(TTF_Font* font, SDL_Renderer* renderer, uint32_t rgb, int x, int y, const char* fmt, ...) {
    SDL_Color text_color;

    VSNPRINT(text, fmt);

    if (font == NULL) {
        font = GAME_DEFAULT_FONT;
    }

    setup_for_text(font, text, rgb, text_color);
    SDL_Surface* message = game_text_surface(text, ::TextRenderMode::BLENDED, font, text_color, text_color);
    safe_render_text_surface(renderer, message, x, y);
}

