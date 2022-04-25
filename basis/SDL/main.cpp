#include <SDL2/SDL.h>   /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */
#include <iostream>     /* C++ 标准输入输出头文件 */

/* 自己的头文件用 双引号 引用 */
#include "rainbow.h"

/*************************************************************************************************/
// 窗口尺寸常量
const int WIN_WIDTH  = 1200;
const int WIN_HEIGHT = 800;

int main(int argc, char* args[]){
    SDL_Window* window = NULL;      // SDL 窗口指针
    SDL_Renderer* renderer = NULL;  // SDL 渲染器指针

    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        // 创建 SDL 窗口和渲染器
        if (SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer) == 0) {
            bool game_is_running = true;    // 游戏主循环标志
            SDL_Event e;                    // SDL 事件

            /** 初始化完成，请开始你的代码 **/
            while(game_is_running) {        // 游戏主循环
                /* 渲染器清零 */
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(renderer);  // 重置窗体

                /** 调用自己头文件里的函数接口 **/
                draw_rainbow(argc, args, window, renderer);
                
                SDL_RenderPresent(renderer);    // 更新窗体

                while (SDL_PollEvent(&e)) { // 处理用户交互事件
                    switch (e.type) {
                    case SDL_QUIT: {
                        std::cout << "Quit after " << e.quit.timestamp << "ms." << std::endl;
                        game_is_running = false;
                    }; break;
                    }
                }
            }
        } else {
            // 创建 SDL 窗口和渲染器失败
            std::cout << "Failed to create the window and renderer: " << std::string(SDL_GetError()) << std::endl;
        }
    } else {
        // SDL 初始化失败
        std::cout << "Failed to initialize the SDL: " << std::string(SDL_GetError()) << std::endl;
    }

    SDL_DestroyRenderer(renderer);  // 销毁 SDL 渲染器
    SDL_DestroyWindow(window);      // 销毁 SDL 窗口
    SDL_Quit();                     // 退出 SDL 系统

    return 0;
}

