#ifndef _RANDOM_WALK_H // 确保只被 include 一次
#define _RANDOM_WALK_H

#include "game.hpp"

/** 声明函数接口 **/
void* random_walk_initialize(int argc, char* argv[], SDL_Window* window, SDL_Renderer* renderer);
void update_random_walk(WarGrey::STEM::timer_frame_t* frame, void* datum, SDL_Renderer* renderer);
void random_walk_exit(void* datum);

#endif

