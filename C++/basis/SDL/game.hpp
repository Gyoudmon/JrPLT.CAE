#ifndef _SYNTAX_GAME_H
#define _SYNTAX_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>

namespace WarGrey::STEM {
    /**********************************************************************************************/
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

    /**********************************************************************************************/
    class Universe {
        public:
            /* 构造函数，创建新对象时自动调用，默认什么都不做 */
            Universe() {}

            /*  更有用一些的构造函数，创建新对象时手动选择，设置窗口标题, 前景背景色, 和混色模式 */
            Universe(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,
                    const char *title, SDL_BlendMode bmode = SDL_BLENDMODE_NONE,
                    uint32_t fgc = 0xFFFFFFFF, uint32_t bgc = 0x000000FF);
            
            /* 析构函数，销毁旧对象时自动调用，默认什么都不做 */
            virtual ~Universe() {}

        public:
            /* 创建游戏世界，充当程序真的 main 函数，默认什么都不做 */
            virtual void construct(int argc, char* argv[], int window_width, int window_height) {}
            
            /* 更新游戏世界，定时器到期时自动调用，默认什么都不做 */
            virtual void update(WarGrey::STEM::timer_frame_t* frame, SDL_Renderer* renderer) {}

        public:
            void set_fps(int fps) { this->_fps = fps; }
            int get_fps() { return this->_fps; }
            uint32_t get_background_color() { return this->_bgc; }
            uint32_t get_foreground_color() { return this->_fgc; }

        public:
            /* 重制屏幕 */
            void clear_screen(SDL_Renderer* renderer);

        private:
            int _fps = 60; // 帧频
            uint32_t _fgc = 0xFFFFFFFF; // 前景色
            uint32_t _bgc = 0x000000FF; // 背景色
    };

    /**********************************************************************************************/
    void game_initialize(uint32_t flags, int fontsize = 16);
    SDL_Texture* game_create_world(int width, int height, SDL_Window** window, SDL_Renderer** renderer);
    uint32_t game_start(uint32_t fps, timer_update_t update_game_world, void* user_datum);
    uint32_t game_start(uint32_t fps, WarGrey::STEM::Universe* universe);
    
    void game_world_reset(SDL_Renderer* renderer, uint32_t fgcolor, uint32_t bgcolor);
    void game_world_reset(SDL_Renderer* renderer, SDL_Texture* texture, uint32_t fgcolor, uint32_t bgcolor);
    void game_world_refresh(SDL_Renderer* renderer, SDL_Texture* texture);
    
    TTF_Font* game_create_font(const char* face, int fontsize);
    void game_font_destroy(TTF_Font* font);

    void game_draw_frame(SDL_Renderer* renderer, int x, int y, int width, int height);
    void game_draw_grid(SDL_Renderer* renderer, int nx, int ny, int grid_size, int xoff = 0, int yoff = 0);
    void game_fill_grid(SDL_Renderer* renderer, int* grids[], int nx, int ny, int grid_size, int xoff = 0, int yoff = 0);

    /**********************************************************************************************/
    extern TTF_Font* GAME_DEFAULT_FONT;
    extern TTF_Font* game_sans_serif_font;
    extern TTF_Font* game_serif_font;
    extern TTF_Font* game_monospace_font;
    extern TTF_Font* game_math_font;
}

#endif

