#ifndef _SYNTAX_SUGAR_H
#define _SYNTAX_SUGAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>

namespace WarGrey::STEM {
    typedef struct timer_frame {
        uint32_t interval;
        uint32_t count;
        uint32_t uptime;
    } timer_frame_t;

    typedef void (*timer_update_t)(timer_frame_t*, void*, SDL_Renderer*);

    typedef struct timer_parcel {
        timer_update_t update_game_world;
        void* user_datum;
        timer_frame_t frame;
    } timer_parcel_t;

    void game_initialize(uint32_t flags);
    SDL_Texture* game_create_world(int width, int height, SDL_Window** window, SDL_Renderer** renderer);
    uint32_t game_start(uint32_t fps, timer_update_t update_game_world, void* user_datum);
    
    void game_world_reset(SDL_Renderer* renderer, SDL_Texture* texture, uint32_t fgcolor, uint32_t bgcolor);
    void game_world_refresh(SDL_Renderer* renderer, SDL_Texture* texture);
}

#endif

