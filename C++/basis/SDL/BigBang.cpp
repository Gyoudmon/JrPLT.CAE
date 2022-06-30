/* 自己的头文件用 双引号 引用, 放最前面以兼容 macOS */
#include "game_of_life.hpp"

#include <iostream>             /* C++ 标准输入输出头文件 */

using namespace WarGrey::STEM;

/*************************************************************************************************/
// 窗口尺寸常量
static const int WIN_WIDTH  = 1200;
static const int WIN_HEIGHT = 800;

/*************************************************************************************************/
int main(int argc, char* args[]) {
    SDL_Window* window = NULL;      // SDL 窗口指针
    SDL_Renderer* renderer = NULL;  // SDL 渲染器指针
    SDL_Texture* texture = NULL;    // SDL 纹理指针
    SDL_TimerID timer = 0;          // SDL 定时器
    
    game_initialize(SDL_INIT_VIDEO | SDL_INIT_TIMER);                       // 初始化游戏系统
    texture = game_create_world(WIN_WIDTH, WIN_HEIGHT, &window, &renderer); // 创建游戏世界

    /* 创建自己的游戏宇宙 */
    auto universe = new HighLife(window, renderer, texture);
    universe->construct(argc, args, WIN_WIDTH, WIN_HEIGHT);
    timer = game_start(universe->get_fps(), reinterpret_cast<timer_update_t>(0LL), reinterpret_cast<void*>(universe));

    /** 初始化完成，请开始你的代码 **/ {
        bool game_is_running = true;        // 游戏主循环标志
        SDL_Event e;                        // SDL 事件

        while(game_is_running) {            // 游戏主循环
            SDL_SetRenderTarget(renderer, texture);
            
            while (SDL_PollEvent(&e)) {     // 处理用户交互事件    
                switch (e.type) {
                case SDL_QUIT: {
                    SDL_RemoveTimer(timer); // 停止定时器
                    delete universe;        // 别忘了销毁游戏世界
                    
                    std::cout << "总计运行了" << e.quit.timestamp / 1000.0F << "秒。" << std::endl;
                    game_is_running = false;
                }; break;
                case SDL_USEREVENT: {       // 收到定时器到期通知，更新游戏
                    timer_parcel_t* parcel = reinterpret_cast<timer_parcel_t*>(e.user.data1);
                    Universe* universe = reinterpret_cast<Universe*>(e.user.data2);
                    timer_frame_t* frame = &(parcel->frame);

                    universe->clear_screen(renderer);
            
                    // TODO: why some first frames are lost, why some frames duplicate.
                    // printf("%u\t%u\n", frame->count, frame->uptime);
                    universe->update(frame, renderer);
                }; break;
                default: {
                    // std::cout << "Ignored unhandled event(type = " << e.type << ")" << std::endl;
                }
                }
            }

            game_world_refresh(renderer, texture); // 更新窗体
        }
    }

    SDL_DestroyTexture(texture);         // 销毁 SDL 纹理
    SDL_DestroyRenderer(renderer);       // 销毁 SDL 渲染器
    SDL_DestroyWindow(window);           // 销毁 SDL 窗口

    return 0;
}

