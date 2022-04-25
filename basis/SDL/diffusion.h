#ifndef _ANIMATED_DIFFUSION_H // 确保只被 include 一次
#define _ANIMATED_DIFFUSION_H

#include "game.h"

/** 声明函数接口 **/
void* diffusion_initialize(int argc, char* argv[], SDL_Window* window, SDL_Renderer* renderer);
void update_diffusion(WarGrey::STEM::timer_frame_t* frame, void* datum, SDL_Renderer* renderer);

#endif

