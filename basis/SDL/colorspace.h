#ifndef COLORSPACE
#define COLORSPACE

#include <SDL2/SDL.h>

namespace WarGrey::STEM {
    int HSV_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double value, double alpha = 1.0);
    int HSL_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double lightness, double alpha = 1.0);
    int HSI_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double intensity, double alpha = 1.0);
}

#endif
