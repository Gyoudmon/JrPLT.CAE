#ifndef COLORSPACE
#define COLORSPACE

#include <SDL2/SDL.h>

namespace WarGrey::STEM {
    int HSV_SetRenderDrawColor(SDL_Renderer* renderer, float hue, float saturation, float value, float alpha = 1.0);
    int HSL_SetRenderDrawColor(SDL_Renderer* renderer, float hue, float saturation, float lightness, float alpha = 1.0);
    int HSI_SetRenderDrawColor(SDL_Renderer* renderer, float hue, float saturation, float intensity, float alpha = 1.0);
}

#endif
