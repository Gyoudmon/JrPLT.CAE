#include "geometry.hpp"
#include "colorspace.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>

using namespace WarGrey::STEM;

/**************************************************************************************************/
static void draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    int err = 2 - 2 * radius;
    int x = -radius;
    int y = 0;
    
    do {
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - y, cy - x);

        radius = err;
        if (radius <= y) {
            err += ++y * 2 + 1;
        }

        if ((radius > x) || (err > y)) {
            err += ++x * 2 ;
        }
    } while (x < 0);
}

static void draw_filled_circle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    int err = 2 - 2 * radius;
    int x = -radius;
    int y = 0;
    
    do {
        SDL_RenderDrawLine(renderer, cx + x, cy + y, cx - x, cy + y); // Q I, Q II
        SDL_RenderDrawLine(renderer, cx + x, cy,     cx + x, cy - y); // Q III
        SDL_RenderDrawLine(renderer, cx - x, cy - y, cx,     cy - y); // Q I

        radius = err;
        if (radius <= y) {
            err += ++y * 2 + 1;
        }

        if ((radius > x) || (err > y)) {
            err += ++x * 2 + 1;
        }
    } while (x < 0);
}

static void draw_ellipse(SDL_Renderer* renderer, int cx, int cy, int ar, int br) {
    aaellipseRGBA(renderer, (int16_t)(cx), (int16_t)(cy), (int16_t)(ar), int16_t(br), 0x12, 0x34, 0x56, 0xFFU);
}

static void draw_filled_ellipse(SDL_Renderer* renderer, int cx, int cy, int ar, int br) {
    filledEllipseRGBA(renderer, (int16_t)(cx), (int16_t)(cy), (int16_t)(ar), int16_t(br), 0x12, 0x34, 0x56, 0xFFU);
}

/**************************************************************************************************/
void WarGrey::STEM::game_draw_point(SDL_Renderer* renderer, int x, int y, uint32_t rgb, uint8_t alpha) {
    unsigned char r, g, b;

    RGB_FromHexadecimal(rgb, &r, &g, &b);
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
    SDL_RenderDrawPoint(renderer, x, y);
}

void WarGrey::STEM::game_draw_point(SDL_Renderer* renderer, int x, int y, float hue, float saturation, float value, uint8_t alpha) {
    HSV_SetRenderDrawColor(renderer, hue, saturation, value, alpha);
    SDL_RenderDrawPoint(renderer, x, y);
}

void WarGrey::STEM::game_draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, uint32_t rgb, uint8_t alpha) {
    unsigned char r, g, b;

    RGB_FromHexadecimal(rgb, &r, &g, &b);
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void WarGrey::STEM::game_draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, float hue, float saturation, float value, uint8_t alpha) {
    HSV_SetRenderDrawColor(renderer, hue, saturation, value, alpha);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void WarGrey::STEM::game_draw_rect(SDL_Renderer* renderer, int x, int y, int width, int height, uint32_t rgb, uint8_t alpha) {
    unsigned char r, g, b;
    SDL_Rect box;

    box.x = x;
    box.y = y;
    box.w = width;
    box.h = height;

    RGB_FromHexadecimal(rgb, &r, &g, &b);
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
    SDL_RenderDrawRect(renderer, &box);
}

void WarGrey::STEM::game_draw_rect(SDL_Renderer* renderer, int x, int y, int width, int height, float hue, float saturation, float value, uint8_t alpha) {
    SDL_Rect box;

    box.x = x;
    box.y = y;
    box.w = width;
    box.h = height;

    HSV_SetRenderDrawColor(renderer, hue, saturation, value, alpha);
    SDL_RenderDrawRect(renderer, &box);
}

void WarGrey::STEM::game_fill_rect(SDL_Renderer* renderer, int x, int y, int width, int height, uint32_t rgb, uint8_t alpha) {
    unsigned char r, g, b;
    SDL_Rect box;

    box.x = x;
    box.y = y;
    box.w = width;
    box.h = height;

    RGB_FromHexadecimal(rgb, &r, &g, &b);
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
    SDL_RenderFillRect(renderer, &box);
}

void WarGrey::STEM::game_fill_rect(SDL_Renderer* renderer, int x, int y, int width, int height, float hue, float saturation, float value, uint8_t alpha) {
    SDL_Rect box;

    box.x = x;
    box.y = y;
    box.w = width;
    box.h = height;

    HSV_SetRenderDrawColor(renderer, hue, saturation, value, alpha);
    SDL_RenderFillRect(renderer, &box);
}

void WarGrey::STEM::game_draw_square(SDL_Renderer* renderer, int cx, int cy, int apothem, uint32_t rgb, uint8_t alpha) {
    game_draw_rect(renderer, cx - apothem, cy - apothem, apothem * 2, apothem * 2, rgb, alpha);
}

void WarGrey::STEM::game_draw_square(SDL_Renderer* renderer, int cx, int cy, int apothem, float hue, float saturation, float value, uint8_t alpha) {
    game_draw_rect(renderer, cx - apothem, cy - apothem, apothem * 2, apothem * 2, hue, saturation, value, alpha);
}

void WarGrey::STEM::game_fill_square(SDL_Renderer* renderer, int cx, int cy, int apothem, uint32_t rgb, uint8_t alpha) {
    game_fill_rect(renderer, cx - apothem, cy - apothem, apothem * 2, apothem * 2, rgb, alpha);
}

void WarGrey::STEM::game_fill_square(SDL_Renderer* renderer, int cx, int cy, int apothem, float hue, float saturation, float value, uint8_t alpha) {
    game_fill_rect(renderer, cx - apothem, cy - apothem, apothem * 2, apothem * 2, hue, saturation, value, alpha);
}

void WarGrey::STEM::game_draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius, uint32_t rgb, uint8_t alpha) {
    unsigned char r, g, b;
    
    RGB_FromHexadecimal(rgb, &r, &g, &b);
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);

    draw_circle(renderer, cx, cy, radius);
}

void WarGrey::STEM::game_draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius, float hue, float saturation, float value, uint8_t alpha) {
    HSV_SetRenderDrawColor(renderer, hue, saturation, value, alpha);
    draw_circle(renderer, cx, cy, radius);
}

void WarGrey::STEM::game_fill_circle(SDL_Renderer* renderer, int cx, int cy, int radius, uint32_t rgb, uint8_t alpha) {
    unsigned char r, g, b;
    
    RGB_FromHexadecimal(rgb, &r, &g, &b);
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);

    draw_filled_circle(renderer, cx, cy, radius);
}

void WarGrey::STEM::game_fill_circle(SDL_Renderer* renderer, int cx, int cy, int radius, float hue, float saturation, float value, uint8_t alpha) {
    HSV_SetRenderDrawColor(renderer, hue, saturation, value, alpha);
    draw_filled_circle(renderer, cx, cy, radius);
}

void WarGrey::STEM::game_draw_ellipse(SDL_Renderer* renderer, int cx, int cy, int ar, int br, uint32_t rgb, uint8_t alpha) {
    unsigned char r, g, b;

    RGB_FromHexadecimal(rgb, &r, &g, &b);
    draw_ellipse(renderer, cx, cy, ar, br);
}

void WarGrey::STEM::game_draw_ellipse(SDL_Renderer* renderer, int cx, int cy, int ar, int br, float hue, float saturation, float value, uint8_t alpha) {
    HSV_SetRenderDrawColor(renderer, hue, saturation, value, alpha);
    draw_ellipse(renderer, cx, cy, ar, br);
}

void WarGrey::STEM::game_fill_ellipse(SDL_Renderer* renderer, int cx, int cy, int ar, int br, uint32_t rgb, uint8_t alpha) {
    unsigned char r, g, b;

    RGB_FromHexadecimal(rgb, &r, &g, &b);
    draw_filled_ellipse(renderer, cx, cy, ar, br);
}

void WarGrey::STEM::game_fill_ellipse(SDL_Renderer* renderer, int cx, int cy, int ar, int br, float hue, float saturation, float value, uint8_t alpha) {
    HSV_SetRenderDrawColor(renderer, hue, saturation, value, alpha);
    draw_filled_ellipse(renderer, cx, cy, ar, br);
}

