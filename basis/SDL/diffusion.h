#ifndef ANIMATED_DIFFUSION // 确保只被 include 一次
#define ANIMATED_DIFFUSION

/** 声明函数接口 **/
void* diffusion_initialize(int argc, char* argv[], SDL_Window* window, SDL_Renderer* renderer);
void update_diffusion(unsigned int count, unsigned int interval, unsigned long long uptime, void* datum, SDL_Renderer* renderer);

#endif

