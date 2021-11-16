#include <iostream>
#include <SDL2/SDL.h>

/*************************************************************************************************/
// 窗口尺寸常量
const int WIN_WIDTH  = 800;
const int WIN_HEIGHT = 600;

// 色块边长常量
const int BLOCK_LENGTH = 200;

/** SDL 程序的入口可以是 `SDL_main`, 也可以是 `main` **/
int main( int argc, char* args[] ){
    SDL_Window* window = NULL;          // SDL 窗口指针
    SDL_Renderer* renderer = NULL;      // SDL 渲染器

    // 初始化 SDL
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    } else {
        // 创建 SDL 窗口和渲染器
        if (SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer)) {
            std::cout << "SDL Error: " << SDL_GetError() << std::endl;
        } else {
            SDL_Rect red, green, blue;

            /** 设置三个色块的尺寸 **/
            red.w = red.h = green.w = green.h = blue.w = blue.h = BLOCK_LENGTH;

            /* 设置窗口标题栏 */
            SDL_SetWindowTitle(window, "SDL");

            /* 渲染器清零 */
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
            SDL_RenderClear(renderer);

            /* 绘制红色色块 */
            red.x = (WIN_WIDTH - red.w) / 2;
            red.y = WIN_HEIGHT / 2 - red.h;
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &red);
            SDL_RenderPresent(renderer);    // 更新窗体
            SDL_Delay(500);                 // 等待一秒种

            /* 绘制绿色色块 */
            green.x = WIN_WIDTH / 2 - green.w + green.w / 4;
            green.y = WIN_HEIGHT / 3;
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &green);
            SDL_RenderPresent(renderer);    // 更新窗体
            SDL_Delay(500);                 // 等待一秒种

            /* 绘制蓝色色块 */
            blue.x = WIN_WIDTH / 2 - blue.w / 4;
            blue.y = green.y;
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderFillRect(renderer, &blue);
            SDL_RenderPresent(renderer);    // 更新窗体
            SDL_Delay(500);                 // 等待一秒种

            SDL_Delay(3500);
        }
    }

    SDL_DestroyRenderer(renderer);  // 销毁 SDL 渲染器
    SDL_DestroyWindow(window);      // 销毁 SDL 窗口
    SDL_Quit();                     // 退出 SDL 系统

    return 0;
}
