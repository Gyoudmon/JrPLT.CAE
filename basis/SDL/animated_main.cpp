#include <SDL2/SDL.h>   /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */
#include <iostream>     /* C++ 标准输入输出头文件 */

/* 自己的头文件用 双引号 引用 */
#include "random_walk.h"

/*************************************************************************************************/
// 窗口尺寸常量
const int WIN_WIDTH  = 1200;
const int WIN_HEIGHT = 800;

// 定时器间隔，60 FPS
const int TIMER_INTERVAL = 1000 / 60;
unsigned int timer_count = 0;
unsigned long long timer_uptime = 0L;
                    
typedef void (*timer_update_t)(unsigned int, unsigned int, unsigned long long, void*, SDL_Renderer*);

/**
 * 本函数在定时器到期时执行, 并将该事件报告给事件系统，以便绘制下一帧动画
 * @param interval, 定时器等待时长，以 ms 为单位
 * @param datum,    用户数据，传递给 SDL_AddTimer 的第三个参数会出现在这
 * @return 返回定时器下次等待时间。注意定时器的实际等待时间是该返回值减去执行该函数所花时间
 **/
static unsigned int trigger_timer_event(unsigned int interval, void* datum) {
    SDL_Event timer_event;
    SDL_UserEvent user_event;

    user_event.type = SDL_USEREVENT;
    user_event.code = 0;

    // 替换成自己头文件里需要执行的函数名称
    user_event.data1 = reinterpret_cast<void*>(update_random_walk);
    // 顺便把用户数据也还给自己的函数
    user_event.data2 = datum;

    timer_count++;
    timer_uptime += interval;
    
    // 将该事件报告给事件系统
    timer_event.type = user_event.type;
    timer_event.user = user_event;
    SDL_PushEvent(&timer_event);

    return interval;
}

int main(int argc, char* args[]){
    SDL_Window* window = NULL;      // SDL 窗口指针
    SDL_Renderer* renderer = NULL;  // SDL 渲染器
    SDL_Texture* texture = NULL;
    void* datum = NULL;        // 传递给定时器的“用户数据”

    // 初始化 SDL 和 定时器
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cout << "Failed to initialize the SDL: " << std::string(SDL_GetError()) << std::endl;
        exit(1);
    }

    atexit(SDL_Quit); // 告诉程序结束前先退出 SDL 系统

    // 创建 SDL 窗口和渲染器
    if (SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer) != 0) {
        std::cout << "Failed to create the window and renderer: " << std::string(SDL_GetError()) << std::endl;
        exit(1);
    }

    // 创建 Texture 作为动画缓存
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);
    if (texture == NULL) {
        std::cout << "Failed to create the texture: " << std::string(SDL_GetError()) << std::endl;
        exit(1);
    }

    // 初始化自己的动画，并将返回值作为用户数据传给定时器
    datum = random_walk_initialize(argc, args, window, renderer);
    if (SDL_AddTimer(TIMER_INTERVAL, trigger_timer_event, datum) == 0) {
        std::cout << "Failed to create the timer: " << std::string(SDL_GetError()) << std::endl;
        exit(1);
    }

    { /** 初始化完成，请开始你的代码 **/
        bool game_is_running = true;     // 游戏主循环标志
        SDL_Event e;                     // SDL 事件

        // 用黑色清空 Texture
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer); 

        // 设置默认颜色为白色
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        while(game_is_running) {         // 游戏主循环
            SDL_SetRenderTarget(renderer, texture);

            while (SDL_PollEvent(&e)) {  // 处理用户交互事件
                switch (e.type) {
                case SDL_QUIT: {
                    std::cout << "Quit after " << e.quit.timestamp << "ms." << std::endl;
                    game_is_running = false;
                }; break;
                case SDL_USEREVENT: {    // 收到定时器到期通知，更新窗体
                    timer_update_t update = reinterpret_cast<timer_update_t>(e.user.data1);
                    update(timer_count, TIMER_INTERVAL, timer_uptime, e.user.data2, renderer);
                }; break;
                default: {
                    // std::cout << "Ignored unhandled event(type = " << e.type << ")" << std::endl;
                }
                }
            }

            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer); // 更新窗体
        }
    }

    SDL_DestroyTexture(texture);         // 销毁 SDL 纹理
    SDL_DestroyRenderer(renderer);       // 销毁 SDL 渲染器
    SDL_DestroyWindow(window);           // 销毁 SDL 窗口

    return 0;
}

