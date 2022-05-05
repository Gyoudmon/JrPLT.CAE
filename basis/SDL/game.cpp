#include "game.hpp"          // 放最前面以兼容 maxOS
#include "colorspace.hpp"

#include <iostream>

using namespace WarGrey::STEM;

/*************************************************************************************************/
#define Call_With_Error_Message(init, message) \
    if (init != 0) { \
        std::cout << message << std::string(SDL_GetError()) << std::endl; \
        exit(1); \
    }

#define Call_With_Safe_Exit(init, message, quit) \
    Call_With_Error_Message(init, message); \
    atexit(quit);

#define Call_For_Variable(id, init, failure, message) \
    id = init; \
    if (id == failure) { \
        std::cout << message << std::string(SDL_GetError()) << std::endl; \
        exit(1); \
    }

/*************************************************************************************************/
/**
 * 本函数在定时器到期时执行, 并将该事件报告给事件系统，以便绘制下一帧动画
 * @param interval, 定时器等待时长，以 ms 为单位
 * @param datum,    用户数据，传递给 SDL_AddTimer 的第三个参数会出现在这
 * @return 返回定时器下次等待时间。注意定时器的实际等待时间是该返回值减去执行该函数所花时间
 **/
static unsigned int trigger_timer_event(unsigned int interval, void* datum) {
    timer_parcel_t* parcel = reinterpret_cast<timer_parcel_t*>(datum);
    SDL_Event timer_event;
    SDL_UserEvent user_event;

    user_event.type = SDL_USEREVENT;
    user_event.code = 0;

    user_event.data1 = datum;
    user_event.data2 = parcel->user_datum;

    parcel->frame.count += 1;
    parcel->frame.interval = interval;
    parcel->frame.uptime = SDL_GetTicks();
    
    // 将该事件报告给事件系统
    timer_event.type = user_event.type;
    timer_event.user = user_event;
    SDL_PushEvent(&timer_event);

    return interval;
}

/*************************************************************************************************/
void WarGrey::STEM::game_initialize(uint32_t flags) {
    Call_With_Safe_Exit(SDL_Init(flags), "SDL 初始化失败: ", SDL_Quit);
    Call_With_Safe_Exit(TTF_Init(), "TTF 初始化失败: ", TTF_Quit);
}

SDL_Texture* WarGrey::STEM::game_create_world(int width, int height, SDL_Window** window, SDL_Renderer** renderer) {
    SDL_Texture* texture;

    Call_With_Error_Message(SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, window, renderer),
        "SDL 窗体和渲染器创建失败: ");
    
    Call_For_Variable(texture,
        SDL_CreateTexture((*renderer), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height),
        NULL, "纹理创建失败: ");

    return texture;
}

uint32_t WarGrey::STEM::game_start(uint32_t fps, timer_update_t update_game_world, void* user_datum) {
    timer_parcel_t* parcel = new timer_parcel_t();
    uint32_t interval = 1000 / fps;
    uint32_t timer;

    // TODO: find a way to delete this parcel
    parcel->update_game_world = update_game_world;
    parcel->user_datum = user_datum;
    parcel->frame.interval = interval;
    parcel->frame.count = 0;
    parcel->frame.uptime = 0;

    Call_For_Variable(timer, SDL_AddTimer(interval, trigger_timer_event, reinterpret_cast<void*>(parcel)),
        0, "定时器创建失败: ");

    return timer;
}

void WarGrey::STEM::game_world_reset(SDL_Renderer* renderer, SDL_Texture* texture, uint32_t fgc, uint32_t bgc) {
    unsigned char r, g, b, a;

    SDL_SetRenderTarget(renderer, texture);

    RGB_FromHexadecimal(bgc, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer); 

    RGB_FromHexadecimal(fgc, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void WarGrey::STEM::game_world_refresh(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

