#include "rainbow.hpp"

#include "flonum.hpp"
#include "mathematics.hpp"
#include "colorspace.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
WarGrey::STEM::ColorWheel::ColorWheel(float r, int blength)
    : DrawingBoard("Color Wheel"), radius(r), block_length(blength) {}

void WarGrey::STEM::ColorWheel::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_Rect hsb; // 色块变量（矩形
    
    hsb.w = hsb.h = this->block_length;

    for (float hue = 0.0F; hue < 360.0F; hue += 10.0F) {
        HSV_SetRenderDrawColor(renderer, hue, 1.0F, 1.0F, 1.0F);
        
        hsb.x = (width - this->block_length)  / 2.0f + this->radius * flcos(degrees_to_radians(hue - 90.0F));
        hsb.y = (height - this->block_length) / 2.0f + this->radius * flsin(degrees_to_radians(hue - 90.0F));
        
        SDL_RenderFillRect(renderer, &hsb);
    }
}

/*************************************************************************************************/
WarGrey::STEM::Rainbow::Rainbow(float r, float dh, float ds)
    : DrawingBoard("Rainbow"), radius(r), hue_delta(dh), sample_delta(ds) {}

void WarGrey::STEM::Rainbow::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    float rainbow_width = this->radius * 2.0f;
    float flx, fly;
    
    for (float hue = 0.0f; hue < 360.0f; hue += this->hue_delta) {
        for (float theta = 0.0f; theta < 360.0f; theta += this->sample_delta) {
            float r = (360.0F - hue) / 360.0F * rainbow_width + this->radius;

            HSV_SetRenderDrawColor(renderer, hue - 75.0F, 1.0F, 1.0F, 1.0F);
        
            flx = float(width)  / 2.0f + r * flcos(degrees_to_radians(theta));
            fly = float(height) / 2.0f + r * flsin(degrees_to_radians(theta));
        
            SDL_RenderDrawPoint(renderer, flx, fly);
        }
    }
}

