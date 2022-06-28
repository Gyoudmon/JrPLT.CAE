#ifndef _ANIMATED_GAME_OF_LIFE_H // 确保只被 include 一次
#define _ANIMATED_GAME_OF_LIFE_H

#include "game.hpp"

/** 声明函数接口 **/
void* game_of_life_initialize(int argc, char* argv[], SDL_Window* window, SDL_Renderer* renderer);
void update_game_of_life(WarGrey::STEM::timer_frame_t* frame, void* datum, SDL_Renderer* renderer);
void game_of_life_exit(void* datum);

#endif

