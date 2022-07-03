#include <SDL2/SDL.h>   /* Simple DirectMedia Layer 头文件, 放前面以兼容 macOS */
#include <iostream>     /* C++ 标准输入输出头文件 */

/* 自己的头文件用 双引号 引用 */
#include "contrast_blocks.hpp"

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
                draw_contrast_blocks(argc, args, window, renderer);
                
                SDL_RenderPresent(renderer);    // 更新窗体

                while (SDL_PollEvent(&e)) { // 处理用户交互事件
                    switch (e.type) {
                    case SDL_QUIT: {
                        std::cout << "总共运行了" << e.quit.timestamp / 1000.0F << "秒。" << std::endl;
                        game_is_running = false;
                    }; break;
                    }
                }
            }
        } else {
            std::cout << "SDL 窗口和渲染器创建失败: " << std::string(SDL_GetError()) << std::endl;
        }
    } else {
        std::cout << "SDL 初始化失败: " << std::string(SDL_GetError()) << std::endl;
    }

    SDL_DestroyRenderer(renderer);  // 销毁 SDL 渲染器
    SDL_DestroyWindow(window);      // 销毁 SDL 窗口
    SDL_Quit();                     // 退出 SDL 系统

    return 0;
}

