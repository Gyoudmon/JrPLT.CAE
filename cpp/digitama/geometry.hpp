#ifndef _SYNTAX_GEOMETRY_H
#define _SYNTAX_GEOMETRY_H

#include <SDL2/SDL.h>

#include <cstdint>
#include <string>

namespace WarGrey::STEM {
    void game_draw_point(SDL_Renderer* renderer, int x, int y, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_draw_point(SDL_Renderer* renderer, int x, int y, float hue, float saturation = 1.0f , float value = 1.0f, uint8_t alpha = 0xFFU);
    void game_draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);
    
    void game_draw_rect(SDL_Renderer* renderer, SDL_Rect* box, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_draw_rect(SDL_Renderer* renderer, SDL_Rect* box, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);
    void game_fill_rect(SDL_Renderer* renderer, SDL_Rect* box, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_fill_rect(SDL_Renderer* renderer, SDL_Rect* box, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);
    void game_draw_rect(SDL_Renderer* renderer, int x, int y, int width, int height, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_draw_rect(SDL_Renderer* renderer, int x, int y, int width, int height, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);
    void game_fill_rect(SDL_Renderer* renderer, int x, int y, int width, int height, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_fill_rect(SDL_Renderer* renderer, int x, int y, int width, int height, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);

    void game_draw_square(SDL_Renderer* renderer, int cx, int cy, int apothem, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_draw_square(SDL_Renderer* renderer, int cx, int cy, int apothem, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);
    void game_fill_square(SDL_Renderer* renderer, int cx, int cy, int apothem, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_fill_square(SDL_Renderer* renderer, int cx, int cy, int apothem, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);

    void game_draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);
    void game_fill_circle(SDL_Renderer* renderer, int cx, int cy, int radius, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_fill_circle(SDL_Renderer* renderer, int cx, int cy, int radius, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);

    void game_draw_ellipse(SDL_Renderer* renderer, int cx, int cy, int aradius, int bradius, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_draw_ellipse(SDL_Renderer* renderer, int cx, int cy, int aradius, int bradius, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);
    void game_fill_ellipse(SDL_Renderer* renderer, int cx, int cy, int aradius, int bradius, uint32_t rgb, uint8_t alpha = 0xFFU);
    void game_fill_ellipse(SDL_Renderer* renderer, int cx, int cy, int aradius, int bradius, float hue, float saturation = 1.0f, float value = 1.0f, uint8_t alpha = 0xFFU);
}

#endif

