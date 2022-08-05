#include "game.hpp"          // 放最前面以兼容 maxOS
#include "vsntext.hpp"
#include "colorspace.hpp"

#include <iostream>
#include <unordered_map>
#include <filesystem>

using namespace WarGrey::STEM;
using namespace std::filesystem;

/*************************************************************************************************/
#define Call_With_Error_Message(init, message, GetError) \
    if (init != 0) { \
        std::cout << message << std::string(GetError()) << std::endl; \
        exit(1); \
    }

#define Call_With_Safe_Exit(init, message, quit, GetError) \
    Call_With_Error_Message(init, message, GetError); \
    atexit(quit);

#define Call_For_Variable(id, init, failure, message, GetError) \
    id = init; \
    if (id == failure) { \
        std::cout << message << std::string(GetError()) << std::endl; \
        exit(1); \
    }

#define Game_Close_Font(id) if (id != NULL) TTF_CloseFont(id); id = NULL

/*************************************************************************************************/
static std::unordered_map<std::string, std::string> system_fonts;
static std::string system_fontdirs[] = {
    "/System/Library/Fonts",
    "/Library/Fonts",
    "C:\\Windows\\Fonts",
    "/usr/share/fonts"
};

TTF_Font* WarGrey::STEM::GAME_DEFAULT_FONT = NULL;
TTF_Font* WarGrey::STEM::game_sans_serif_font = NULL;
TTF_Font* WarGrey::STEM::game_serif_font = NULL;
TTF_Font* WarGrey::STEM::game_monospace_font = NULL;
TTF_Font* WarGrey::STEM::game_math_font = NULL;

static void game_push_fonts_of_directory(std::filesystem::path& root) {
    for (auto entry : directory_iterator(root)) {
        path self = entry.path();

        if (entry.is_directory()) {
            game_push_fonts_of_directory(self);
        } else if (entry.is_regular_file()) {
            system_fonts[self.filename().string()] = self.string();
        }
    }
}

static void game_fonts_initialize(int fontsize = 16) {
    for (unsigned int idx = 0; idx < sizeof(system_fontdirs) / sizeof(std::string); idx++) {
        path root(system_fontdirs[idx]);

        if (exists(root) && is_directory(root)) {
            game_push_fonts_of_directory(root);
        }
    }

#if defined(__macosx__)
    game_sans_serif_font = game_create_font("LucidaGrande.ttc", fontsize);
    game_serif_font = game_create_font("Times.ttc", fontsize);
    game_monospace_font = game_create_font("Courier.ttc", fontsize);
    game_math_font = game_create_font("Bodoni 72.ttc", fontsize);
#elif defined(__windows__) /* HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Fonts */
    game_sans_serif_font = game_create_font("msyh.ttc", fontsize); // Microsoft YaHei
    game_serif_font = game_create_font("times.ttf", fontsize); // Times New Roman
    game_monospace_font = game_create_font("cour.ttf", fontsize); // Courier New
    game_math_font = game_create_font("BOD_R.TTF", fontsize); // Bodoni MT
#else /* the following fonts have not been tested */
    game_sans_serif_font = game_create_font("Nimbus Sans.ttc", fontsize);
    game_serif_font = game_create_font("DejaVu Serif.ttc", fontsize);
    game_monospace_font = game_create_font("Monospace.ttf", fontsize);
    game_math_font = game_create_font("URW Bookman.ttf", fontsize);
#endif

    GAME_DEFAULT_FONT = game_monospace_font;
}

static void game_fonts_destroy() {
    Game_Close_Font(GAME_DEFAULT_FONT);
}

/*************************************************************************************************/
typedef struct timer_parcel {
    Universe* universe;
    uint32_t interval;
    uint32_t count;
    uint32_t uptime;
} timer_parcel_t;

/**
 * 本函数在定时器到期时执行, 并将该事件报告给事件系统，以便绘制下一帧动画
 * @param interval, 定时器等待时长，以 ms 为单位
 * @param datum,    用户数据，传递给 SDL_AddTimer 的第三个参数会出现在这
 * @return 返回定时器下次等待时间。注意定时器的实际等待时间是该返回值减去执行该函数所花时间
 **/
static unsigned int trigger_timer_event(unsigned int interval, void* datum) {
    timer_parcel_t* parcel = reinterpret_cast<timer_parcel_t*>(datum);
    SDL_UserEvent user_event;
    SDL_Event timer_event;

    user_event.type = SDL_USEREVENT;
    user_event.code = 0;

    user_event.data1 = datum;

    parcel->count += 1;
    parcel->interval = interval;
    parcel->uptime = SDL_GetTicks();

    // 将该事件报告给事件系统
    timer_event.type = user_event.type;
    timer_event.user = user_event;
    SDL_PushEvent(&timer_event);

    return interval;
}

/*************************************************************************************************/
void WarGrey::STEM::game_initialize(uint32_t flags, int fontsize) {
    if (GAME_DEFAULT_FONT == NULL) {
        Call_With_Safe_Exit(SDL_Init(flags), "SDL 初始化失败: ", SDL_Quit, SDL_GetError);
        Call_With_Safe_Exit(TTF_Init(), "TTF 初始化失败: ", TTF_Quit, TTF_GetError);

        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    
        game_fonts_initialize(fontsize);

        atexit(IMG_Quit);
        atexit(game_fonts_destroy);
    }
}

SDL_Texture* WarGrey::STEM::game_create_world(int width, int height, SDL_Window** window, SDL_Renderer** renderer) {
    SDL_Texture* texture;

    Call_With_Error_Message(SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, window, renderer),
        "SDL 窗体和渲染器创建失败: ", SDL_GetError);

    Call_For_Variable(texture,
        SDL_CreateTexture((*renderer), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height),
        NULL, "纹理创建失败: ", SDL_GetError);

    return texture;
}

void WarGrey::STEM::game_world_reset(SDL_Renderer* renderer, uint32_t fgc, uint32_t bgc) {
    unsigned char r, g, b, a;

    RGB_FromHexadecimal(bgc, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);

    RGB_FromHexadecimal(fgc, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void WarGrey::STEM::game_world_reset(SDL_Renderer* renderer, SDL_Texture* texture, uint32_t fgc, uint32_t bgc) {
    SDL_SetRenderTarget(renderer, texture);
    game_world_reset(renderer, fgc, bgc);
}

void WarGrey::STEM::game_world_refresh(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

/*************************************************************************************************/
void WarGrey::STEM::game_draw_frame(SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_Rect box;

    box.x = x - 1;
    box.y = y - 1;
    box.w = width + 3;
    box.h = height + 3;

    SDL_RenderDrawRect(renderer, &box);
}

void WarGrey::STEM::game_draw_grid(SDL_Renderer* renderer, int nx, int ny, int grid_size, int xoff, int yoff) {
    int xend = xoff + nx * grid_size;
    int yend = yoff + ny * grid_size;

    for (int i = 0; i <= nx; i++) {
        int x = xoff + i * grid_size;

        for (int j = 0; j <= ny; j++) {
            int y = yoff + j * grid_size;

            SDL_RenderDrawLine(renderer, xoff, y, xend, y);
        }

        SDL_RenderDrawLine(renderer, x, yoff, x, yend);
    }
}

void WarGrey::STEM::game_fill_grid(SDL_Renderer* renderer, int* grids[], int nx, int ny, int grid_size, int xoff, int yoff) {
    SDL_Rect grid_self;

    grid_self.w = grid_size;
    grid_self.h = grid_size;

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            if (grids[i][j] > 0) {
                grid_self.x = xoff + i * grid_self.w;
                grid_self.y = yoff + j * grid_self.h;
                SDL_RenderFillRect(renderer, &grid_self);
            }
        }
    }
}

void WarGrey::STEM::game_render_surface(SDL_Renderer* target, SDL_Surface* surface, int x, int y) {
    SDL_Rect box;

    box.x = x;
    box.y = y;
    box.w = surface->w;
    box.h = surface->h;

    game_render_surface(target, surface, &box);
}

void WarGrey::STEM::game_render_surface(SDL_Renderer* target, SDL_Surface* surface, SDL_Rect* region) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(target, surface);

    if (texture != NULL) {
        SDL_RenderCopy(target, texture, NULL, region);
        SDL_DestroyTexture(texture);
    }
}

/*************************************************************************************************/
TTF_Font* WarGrey::STEM::game_create_font(const char* face, int fontsize) {
    std::string face_key(face);
    TTF_Font* font = NULL;

    if (system_fonts.find(face_key) == system_fonts.end()) {
        font = TTF_OpenFont(face, fontsize);
    } else {
        font = TTF_OpenFont(system_fonts[face_key].c_str(), fontsize);
    }

    if (font == NULL) {
        fprintf(stderr, "无法加载字体 '%s': %s\n", face, TTF_GetError());
    }

    return font;
}

void WarGrey::STEM::game_font_destroy(TTF_Font* font) {
    TTF_CloseFont(font);
}

/*************************************************************************************************/
void WarGrey::STEM::game_draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius, uint32_t color) {
    unsigned char r, g, b, a;

    RGB_FromHexadecimal(color, &r, &g, &b, &a);
    aaellipseRGBA(renderer, (int16_t)(cx), (int16_t)(cy), (int16_t)(radius), int16_t(radius), r, g, b, a);
}

void WarGrey::STEM::game_fill_circle(SDL_Renderer* renderer, int cx, int cy, int radius, uint32_t color) {
    unsigned char r, g, b, a;

    RGB_FromHexadecimal(color, &r, &g, &b, &a);
    filledEllipseRGBA(renderer, (int16_t)(cx), (int16_t)(cy), (int16_t)(radius), int16_t(radius), r, g, b, a);
}

void WarGrey::STEM::game_draw_rectangle(SDL_Renderer* renderer, int x, int y, int width, int height, uint32_t color) {
    unsigned char r, g, b, a;

    RGB_FromHexadecimal(color, &r, &g, &b, &a);
    rectangleRGBA(renderer, (int16_t)x, (int16_t)y, (int16_t)width, (int16_t)height, r, g, b, a);
}

void WarGrey::STEM::game_fill_rectangle(SDL_Renderer* renderer, int x, int y, int width, int height, uint32_t color) {
    unsigned char r, g, b, a;

    RGB_FromHexadecimal(color, &r, &g, &b, &a);
    boxRGBA(renderer, (int16_t)x, (int16_t)y, (int16_t)width, (int16_t)height, r, g, b, a);
}

/*************************************************************************************************/
WarGrey::STEM::Universe::Universe() : Universe("Big Bang!", 1200, 800) {}

WarGrey::STEM::Universe::Universe(const char *title, int width, int height, int fps, uint32_t fgc, uint32_t bgc)
    : _fps(fps), _fgc(fgc), _bgc(bgc) {
    
    // 初始化游戏系统
    game_initialize(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    // 创建游戏世界
    this->texture = game_create_world(width, height, &this->window, &this->renderer);

    // 设置标题
    SDL_SetWindowTitle(this->window, title);
    game_world_reset(this->renderer, this->texture, this->_fgc, this->_bgc);

    this->set_blend_mode(SDL_BLENDMODE_NONE);
}

WarGrey::STEM::Universe::~Universe() {
    if (this->timer > 0) {
        SDL_RemoveTimer(this->timer);
    }

    SDL_DestroyTexture(this->texture);
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
}

uint32_t WarGrey::STEM::Universe::big_bang() {
    uint32_t quit_time = 0UL;           // 游戏退出时的在线时间
    SDL_Event e;                        // SDL 事件
    int width, height;
    timer_parcel_t parcel;
    
    if (this->_fps > 0) {
        parcel.universe = this;
        parcel.interval = 1000 / this->_fps;
        parcel.count = 0;
        parcel.uptime = 0;

        this->timer = Call_For_Variable(timer,
                SDL_AddTimer(parcel.interval, trigger_timer_event, reinterpret_cast<void*>(&parcel)),
                0, "定时器创建失败: ", SDL_GetError);
    }

    this->fill_window_size(&width, &height);
    this->draw(this->renderer, 0, 0, width, height);

    while(quit_time == 0UL) {            // 游戏主循环
        SDL_SetRenderTarget(this->renderer, this->texture);
            
        while (SDL_PollEvent(&e)) {     // 处理用户交互事件    
            switch (e.type) {
            case SDL_USEREVENT: {       // 定时器到期通知，更新游戏
                auto parcel = reinterpret_cast<timer_parcel_t*>(e.user.data1);

                if (parcel->universe == this) {
                    this->on_elapse(parcel->interval, parcel->count, parcel->uptime);
                    this->draw(this->renderer, 0, 0, width, height);
                }
            }; break;
            case SDL_MOUSEMOTION: {     // 鼠标移动事件
                this->on_mouse_event(e.motion);
            }; break;
            case SDL_MOUSEWHEEL: {      // 鼠标滚轮事件
                this->on_mouse_event(e.wheel);
            }; break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN: { // 鼠标点击事件
                this->on_mouse_event(e.button);
            }; break;
            case SDL_KEYUP:
            case SDL_KEYDOWN: {         // 键盘事件
                this->on_keyboard_event(e.key);
            }; break;
            case SDL_QUIT: {
                if (this->timer > 0UL) {
                    SDL_RemoveTimer(this->timer); // 停止定时器
                    this->timer = 0;
                }

                quit_time = e.quit.timestamp;
            }; break;
            default: {
                // std::cout << "Ignored unhandled event(type = " << e.type << ")" << std::endl;
            }
            }

            game_world_refresh(this->renderer, this->texture); // 更新窗体
        }
    }

    return quit_time;
}

void WarGrey::STEM::Universe::on_frame(uint32_t interval, uint32_t count, uint32_t uptime) {
    game_world_reset(this->renderer, this->_fgc, this->_bgc);
}

void WarGrey::STEM::Universe::on_elapse(uint32_t interval, uint32_t count, uint32_t uptime) {
    this->update(interval, count, uptime);
    this->on_frame(interval, count, uptime);
}

void WarGrey::STEM::Universe::on_mouse_event(SDL_MouseButtonEvent &mouse) {
    if (mouse.state == SDL_RELEASED) {
        if (mouse.clicks == 1) {
            switch (mouse.button) {
                case SDL_BUTTON_LEFT: this->on_click(mouse.x, mouse.y); break;
                case SDL_BUTTON_RIGHT: this->on_right_click(mouse.x, mouse.y); break;
            }
        } else {
            switch (mouse.button) {
                case SDL_BUTTON_LEFT: this->on_double_click(mouse.x, mouse.y); break;
            }
        }
    }
}

void WarGrey::STEM::Universe::on_mouse_event(SDL_MouseMotionEvent &mouse) {
    this->on_mouse_move(mouse.state, mouse.x, mouse.y, mouse.xrel, mouse.yrel);
}

void WarGrey::STEM::Universe::on_mouse_event(SDL_MouseWheelEvent &mouse) {
    int horizon = mouse.x;
    int vertical = mouse.y;
    float hprecise = float(horizon);  // mouse.preciseX;
    float vprecise = float(vertical); // mouse.preciseY;

    if (mouse.direction == SDL_MOUSEWHEEL_FLIPPED) {
        horizon  *= -1;
        vertical *= -1;
        hprecise *= -1.0F;
        vprecise *= -1.0F;
    }

    this->on_scroll(horizon, vertical, hprecise, vprecise);
}

void WarGrey::STEM::Universe::on_keyboard_event(SDL_KeyboardEvent &keyboard) {
    if (keyboard.state == SDL_PRESSED) {
        SDL_Keysym key = keyboard.keysym;

        this->on_char(key.sym, key.mod, keyboard.repeat);
    }
}

/*************************************************************************************************/
void WarGrey::STEM::Universe::set_blend_mode(SDL_BlendMode bmode) {
    SDL_SetRenderDrawBlendMode(this->renderer, bmode);
}

void WarGrey::STEM::Universe::set_window_title(std::string& title) {
    SDL_SetWindowTitle(this->window, title.c_str());
}

void WarGrey::STEM::Universe::set_window_title(const char* fmt, ...) {
    VSNPRINT(title, fmt);
    this->set_window_title(title);
}

void WarGrey::STEM::Universe::fill_window_size(int* width, int* height) {
    SDL_GetWindowSize(this->window, width, height);
}

