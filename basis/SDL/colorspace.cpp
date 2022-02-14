#include <SDL2/SDL.h>   /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */
#include <cmath>

#include "colorspace.h"

#include "flonum.h"
#include "fixnum.h"

using namespace WarGrey::STEM;

/*************************************************************************************************/
#define UCHAR(v) ((unsigned char)flround(v * 255.0))

static int sdl_set_render_draw_color(SDL_Renderer* renderer, double r, double g, double b, double a) {
    return SDL_SetRenderDrawColor(renderer, UCHAR(r), UCHAR(g), UCHAR(b), UCHAR(a));
}

/*************************************************************************************************/
static int set_render_color_from_hue(SDL_Renderer* renderer, double hue, double chroma, double m, double a) {
    double r = m;
    double g = m;
    double b = m;
    
    if (!flisnan(hue)) {
        double hue_60 = hue / 60.0;
        double flhue = flfloor(hue_60);
        int fxhue = int(flhue);
        double x = chroma * (1.0 - flabs(double(fxhue % 2) - (flhue - hue_60) - 1.0));
        
        switch (fxhue) {
        case 0: r += chroma; g += x; break;
        case 1: r += x; g += chroma; break;
        case 2: g += chroma; b += x; break;
        case 3: g += x; b += chroma; break;
        case 4: r += x; b += chroma; break;
        case 5: r += chroma; b += x; break;
        }
    }

    return sdl_set_render_draw_color(renderer, r, g, b, a);
}

static int set_render_color_from_hsi_sector(SDL_Renderer* renderer, double hue, double saturation, double intensity, char color_component, double alpha) {
    double cosH_60H = 2.0; // if hue == 0.0 or hue == 120.0;

    if ((hue != 0.0) && (hue != 120.0)) {
        double H = hue * (pi / 180.0);
        cosH_60H = flcos(H) / flcos(pi / 3.0 - H);
    }

    {
        double major = intensity * (1.0 + saturation * cosH_60H);
        double midor = intensity * (1.0 - saturation);
        double minor = (intensity * 3.0) - (major + midor);

        switch (color_component) {
        case 'r': return sdl_set_render_draw_color(renderer, major, minor, midor, alpha); break;
        case 'g': return sdl_set_render_draw_color(renderer, midor, major, minor, alpha); break;
        default:  return sdl_set_render_draw_color(renderer, minor, midor, major, alpha); break;
        }
    }
}

/*************************************************************************************************/
int WarGrey::STEM::HSV_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double value, double alpha) {
    double chroma = saturation * value;
    double m = value - chroma;
    
    return set_render_color_from_hue(renderer, hue, chroma, m, alpha);
}

/*************************************************************************************************/
int WarGrey::STEM::HSL_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double lightness, double alpha) {
    double chroma = saturation * (1.0 - flabs(lightness * 2.0 - 1.0));
    double m = lightness - chroma * 0.5;
    
    return set_render_color_from_hue(renderer, hue, chroma, m, alpha);
}

/*************************************************************************************************/
int WarGrey::STEM::HSI_SetRenderDrawColor(SDL_Renderer* renderer, double hue, double saturation, double intensity, double alpha) {
    if ((saturation == 0.0) || flisnan(saturation)) {
        return sdl_set_render_draw_color(renderer, intensity, intensity, intensity, alpha);
    } else if (hue < 120.0) {
        return set_render_color_from_hsi_sector(renderer, hue, saturation, intensity, 'r', alpha);
    } else if (hue < 240.0) {
        return set_render_color_from_hsi_sector(renderer, hue - 120.0, saturation, intensity, 'g', alpha);
    } else {
        return set_render_color_from_hsi_sector(renderer, hue - 240.0, saturation, intensity, 'b', alpha);
    }
}
