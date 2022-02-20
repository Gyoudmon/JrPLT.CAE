#ifndef ANIMATED_RANDOM_WALK // 确保只被 include 一次
#define ANIMATED_RANDOM_WALK

/** 声明函数接口 **/
void* random_walk_initialize(int argc, char* argv[], SDL_Window* window, SDL_Renderer* renderer);
void update_random_walk(unsigned int count, unsigned int interval, unsigned long long uptime, void* datum, SDL_Renderer* renderer);

#endif

