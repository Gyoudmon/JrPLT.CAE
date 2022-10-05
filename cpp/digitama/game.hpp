#ifndef _SYNTAX_GAME_H
#define _SYNTAX_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <cstdint>
#include <string>

#include "named_colors.hpp"
#include "geometry.hpp"

/**
 * 为兼容 Windows 而变得丑陋
 *
 * vcpkg 不直接提供 SDL2main
 * SDL2.h 把 main 改名为 SDL_main 了
 * 导致 MSVC 连接器找不到程序入口
 *
 * 手动链接会引发别的问题，比如跟默认库冲突
 * (否则 vcpkg 作者肯定脑子有坑才会整出“手动链接”这种脱了裤子那啥的活)
 */
#ifdef main
#undef main
#endif

namespace WarGrey::STEM {
    /**********************************************************************************************/
    class Universe {
        public:
            /* 构造函数，创建新对象时自动调用，默认创建一个黑底白字的窗口 */
            Universe();

            /**
             * 更有用一些的构造函数，创建新对象时根据参数自动选择，
             * 设置帧频, 窗口标题, 前景背景色, 和混色模式
             */
            Universe(const char* title, int fps = 60, uint32_t fgc = 0xFFFFFFU, uint32_t bgc = 0x000000U);
            
            /* 析构函数，销毁旧对象时自动调用，默认销毁游戏宇宙 */
            virtual ~Universe();

            /* 宇宙大爆炸，开始游戏主循环 */
            void big_bang();

        public:
            /* 创建游戏世界，充当程序真正的 main 函数，默认什么都不做 */
            virtual void construct(int argc, char* argv[]) {}
            
            /* 排列可视化元素，在合适的时候自动调用，默认什么都不做 */
            virtual void reflow(int width, int height) {}
            
            /* 更新游戏世界，定时器到期时自动调用，默认什么都不做 */
            virtual void update(uint32_t interval, uint32_t count, uint32_t uptime) {}
            
            /* 绘制游戏世界，在合适的时候自动调用，默认什么都不做 */
            virtual void draw(SDL_Renderer* renderer, int x, int y, int width, int height) {}

            /* 告诉游戏主循环，是否游戏已经结束可以退出了，默认永久运行 */
            virtual bool can_exit() { return false; }
            
        public:
            void set_blend_mode(SDL_BlendMode bmode);
            void set_window_title(std::string& title);
            void set_window_title(const char* fmt, ...);
            void set_window_size(int width, int height, bool centerize = true);
            void fill_window_size(int* width, int* height);
            void set_window_fullscreen(bool yes);
            int get_frame_per_second() { return this->_fps; }
            uint32_t get_background_color() { return this->_bgc; }
            uint32_t get_foreground_color() { return this->_fgc; }

        public:
            void set_input_echo_area(int x, int y, int width, int height, int fgc = -1, int bgc = -1);
            bool start_input_text();
            bool stop_input_text();
            bool enter_input_text();
            bool popback_input_text();

        protected:
            virtual bool on_click(int x, int y) { return false; }                                               // 处理单击事件
            virtual bool on_right_click(int x, int y) { return false; }                                         // 处理右击事件
            virtual bool on_double_click(int x, int y) { return false; }                                        // 处理双击事件
            virtual bool on_mouse_move(uint32_t state, int x, int y, int dx, int dy) { return false; }          // 处理移动事件
            virtual bool on_scroll(int horizon, int vertical, float hprecise, float vprecise) { return false; } // 处理滚轮事件

            virtual bool on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) { return false; } // 处理键盘事件
            virtual bool on_text(const char* text, bool entire) { return false; }                               // 处理文本输入事件
            virtual bool on_text(const char* text, int pos, int span) { return false; }                         // 处理文本输入事件

        protected:
            virtual void on_frame(uint32_t interval, uint32_t count, uint32_t uptime);

        private:
            /* 响应定时器事件，刷新游戏世界 */
            void on_elapse(uint32_t interval, uint32_t count, uint32_t uptime);

            /* 响应鼠标事件，并按需调用单击、右击、双击、移动、滚轮事件 */
            bool on_mouse_event(SDL_MouseButtonEvent &mouse, bool pressed); 
            bool on_mouse_event(SDL_MouseMotionEvent &mouse); 
            bool on_mouse_event(SDL_MouseWheelEvent &mouse);

            /* 响应键盘事件，并按需调按下、松开事件 */
            bool on_keyboard_event(SDL_KeyboardEvent &key, bool pressed);

            /* 响应窗体事件，并按需调用尺寸改变事件 */
            bool on_resize(int width, int height);

            /* 响应输入法事件，按需显示用户输入的内容 */
            bool on_user_input(const char* text);
            bool on_editing(const char* text, int pos, int span);

        private:
            void do_redraw(int x, int y, int width, int height);
            bool display_usr_input_and_caret(bool yes);

        private:
            uint32_t _fgc = 0xFFFFFFU;      // 窗体前景色
            uint32_t _bgc = 0x000000U;      // 窗体背景色
            SDL_Window* window = NULL;      // 窗体对象
            SDL_Renderer* renderer = NULL;  // 渲染器对象
            SDL_Texture* texture = NULL;    // 纹理对象

        private:
            SDL_TimerID timer = 0;          // SDL 定时器
            int _fps = 60;                  // 帧频

        private:
            const char* current_usrin;      // IME 原始输入
            std::string usrin;              // 用户输入
            bool in_editing;                // 是否在输入期间
            SDL_Rect echo;                  // 输入回显区域
            uint32_t _ifgc;                 // 回显区前景色
            uint32_t _ibgc;                 // 回显区背景色
    };

    class DrawingBoard : public WarGrey::STEM::Universe {
        public:
            DrawingBoard(const char* title, uint32_t fgc = 0x000000U, uint32_t bgc = 0xFFFFFFU)
                : Universe(title, 0, fgc, bgc) {}
    };

    class DrawingPlayer : public WarGrey::STEM::Universe {
        public:
            DrawingPlayer(const char* title, int fps = 24, uint32_t fgc = 0xFFFFFFU, uint32_t bgc = 0x000000U)
                : Universe(title, fps, fgc, bgc) {}
            
        protected:
            /* 绘制新帧时不擦除已有帧 */
            void on_frame(uint32_t interval, uint32_t count, uint32_t uptime) {}
    };

    /**********************************************************************************************/
    void game_initialize(uint32_t flags, int fontsize = 16);
    SDL_Texture* game_create_world(int width, int height, SDL_Window** window, SDL_Renderer** renderer);
    SDL_Texture* game_create_texture(SDL_Window* window, SDL_Renderer* renderer);
    
    void game_world_reset(SDL_Renderer* renderer, uint32_t fgcolor, uint32_t bgcolor);
    void game_world_reset(SDL_Renderer* renderer, SDL_Texture* texture, uint32_t fgcolor, uint32_t bgcolor);
    void game_world_refresh(SDL_Renderer* renderer, SDL_Texture* texture);
    
    void game_draw_frame(SDL_Renderer* renderer, int x, int y, int width, int height);
    void game_draw_grid(SDL_Renderer* renderer, int nx, int ny, int grid_size, int xoff = 0, int yoff = 0);
    void game_fill_grid(SDL_Renderer* renderer, int* grids[], int nx, int ny, int grid_size, int xoff = 0, int yoff = 0);

    void game_render_surface(SDL_Renderer* target, SDL_Surface* surface, int x, int y);
    void game_render_surface(SDL_Renderer* target, SDL_Surface* surface, SDL_Rect* region);

    /**********************************************************************************************/
    extern TTF_Font* GAME_DEFAULT_FONT;
    extern TTF_Font* game_sans_serif_font;
    extern TTF_Font* game_serif_font;
    extern TTF_Font* game_monospace_font;
    extern TTF_Font* game_math_font;
    extern TTF_Font* game_unicode_font;

    TTF_Font* game_create_font(const char* face, int fontsize);
    void game_font_destroy(TTF_Font* font);
    const std::string* game_font_list(int* n, int fontsize = 16);
}

#endif

