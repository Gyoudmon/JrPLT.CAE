#ifndef COLORSPACE
#define COLORSPACE

#include <SDL2/SDL.h>

namespace WarGrey::STEM {
    int HSV_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double value, unsigned char alpha = 0xFF);
    int HSL_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double lightness, unsigned char alpha = 0xFF);
    int HSI_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double intensity, unsigned char alpha = 0xFF);
}

#endif
