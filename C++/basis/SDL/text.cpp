#include "game.hpp"                 // 放最前面以兼容 macOS
#include "text.hpp"
#include "colorspace.hpp"

#include <cstdarg>
#include <iostream>

using namespace WarGrey::STEM;

/*************************************************************************************************/
enum TextRenderMode { SOLID, SHADED, BLENDED };

/*************************************************************************************************/
#define VSNPRINT(retval, fmt) \
    const int DEFAULT_POOL_SIZE = 1024; \
    char chpool[DEFAULT_POOL_SIZE]; \
    int bigSize = DEFAULT_POOL_SIZE - 1; \
    char* pool; \
    va_list argl; \
    do { \
	pool = (bigSize < DEFAULT_POOL_SIZE) ? chpool : (new char[bigSize + 1]); \
    	va_start(argl, fmt); \
    	int status = vsnprintf(pool, bigSize + 1, fmt, argl); \
	va_end(argl); \
        if ((status == -1) || (status > bigSize)) { \
	    bigSize = ((status > 0) ? status : (bigSize * 2)) + 1; \
	    if (pool != chpool) delete[] pool; \
	    pool = nullptr; \
	} \
    } while (pool == nullptr); \
    std::string retval(pool); \
    if (pool != chpool) delete[] pool;

/*************************************************************************************************/
static inline void unsafe_utf8_size(TTF_Font* font, int* width, int* height, const std::string& text) {
    if (TTF_SizeUTF8(font, text.c_str(), width, height)) {
        std::cerr << "无法计算文本尺寸: " << TTF_GetError() << std::endl;
    }
}

static inline void setup_for_text(TTF_Font* font, const std::string& text, unsigned int rgba, int x, int y, SDL_Rect& r, SDL_Color& c) {
    RGB_FromHexadecimal(rgba, &c.r, &c.g, &c.b, &c.a);

    r.x = x;
    r.y = y;
    unsafe_utf8_size(font, &r.w, &r.h, text);
}

static SDL_Surface* game_text_surface(const std::string& text, ::TextRenderMode mode, TTF_Font* font, SDL_Color& fgc, SDL_Color& bgc) {
    SDL_Surface* surface = NULL;

    switch (mode) {
        case ::TextRenderMode::SOLID: surface = TTF_RenderUTF8_Solid(font, text.c_str(), fgc); break;
        case ::TextRenderMode::SHADED: surface = TTF_RenderUTF8_Shaded(font, text.c_str(), fgc, bgc); break;
        case ::TextRenderMode::BLENDED: surface = TTF_RenderUTF8_Blended(font, text.c_str(), fgc); break;
    }

    if (surface == NULL) {
        std::cerr << "无法渲染文本: " << TTF_GetError() << std::endl;
    }

    return surface;
}

static inline void safe_render_text_surface(SDL_Renderer* target, SDL_Surface* message, SDL_Rect* region) {
    /** TODO: Cache the textures of text **/
    
    SDL_Texture* text = SDL_CreateTextureFromSurface(target, message);

    if (text != NULL) {
        SDL_RenderCopy(target, text, NULL, region);
        SDL_DestroyTexture(text);
    }

    SDL_FreeSurface(message);
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
    
void WarGrey::STEM::game_draw_solid_text(TTF_Font* font, SDL_Renderer* renderer, uint32_t rgba, int x, int y, const char* fmt, ...) {
    SDL_Rect text_region;
    SDL_Color text_color;

    VSNPRINT(text, fmt);

    if (font == NULL) {
        font = GAME_DEFAULT_FONT;
    }

    setup_for_text(font, text, rgba, x, y, text_region, text_color);
    SDL_Surface* message = game_text_surface(text, ::TextRenderMode::SOLID, font, text_color, text_color);
    safe_render_text_surface(renderer, message, &text_region);
}

void WarGrey::STEM::game_draw_shaded_text(TTF_Font* font, SDL_Renderer* renderer, uint32_t fgc, uint32_t bgc, int x, int y, const char* fmt, ...) {
    SDL_Rect text_region;
    SDL_Color text_color, background_color;

    VSNPRINT(text, fmt);

    if (font == NULL) {
        font = GAME_DEFAULT_FONT;
    }

    setup_for_text(font, text, fgc, x, y, text_region, text_color);
    RGB_FromHexadecimal(bgc, &background_color.r, &background_color.g, &background_color.b, &background_color.a);
    SDL_Surface* message = game_text_surface(text, ::TextRenderMode::SHADED, font, text_color, background_color);
    safe_render_text_surface(renderer, message, &text_region);
}

void WarGrey::STEM::game_draw_blended_text(TTF_Font* font, SDL_Renderer* renderer, uint32_t rgba, int x, int y, const char* fmt, ...) {
    SDL_Rect text_region;
    SDL_Color text_color;

    VSNPRINT(text, fmt);

    if (font == NULL) {
        font = GAME_DEFAULT_FONT;
    }

    setup_for_text(font, text, rgba, x, y, text_region, text_color);
    SDL_Surface* message = game_text_surface(text, ::TextRenderMode::BLENDED, font, text_color, text_color);
    safe_render_text_surface(renderer, message, &text_region);
}

