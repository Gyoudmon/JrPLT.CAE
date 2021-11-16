#include <iostream>
#include <SDL.h>

/*************************************************************************************************/
// 窗口尺寸常量
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

int SDL_main( int argc, char* args[] ){
    SDL_Window* window = NULL;          // SDL 窗口
    SDL_Surface* screenSurface = NULL;  // SDL 窗口里的屏幕

    // 初始化 SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        // 创建 SDL 窗口
        window = SDL_CreateWindow( "Hello, RGB", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        if ( window == NULL ) {
            std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
        } else {
            screenSurface = SDL_GetWindowSurface( window ); // 将“窗口屏幕”赋值给变量 screenSurface
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0xFF, 0xFF ) ); // 给屏幕填色

            SDL_UpdateWindowSurface( window );  // 更新窗体
            SDL_Delay( 2000 );                  // 等待两秒种
        }
    }

    SDL_DestroyWindow( window );    // 销毁 SDL 窗口
    SDL_Quit();                     // 退出 SDL 系统

    return 0;
}
