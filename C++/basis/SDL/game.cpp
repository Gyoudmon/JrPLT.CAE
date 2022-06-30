#include "game.hpp"          // 放最前面以兼容 maxOS
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
void WarGrey::STEM::game_initialize(uint32_t flags, int fontsize) {
    Call_With_Safe_Exit(SDL_Init(flags), "SDL 初始化失败: ", SDL_Quit, SDL_GetError);
    Call_With_Safe_Exit(TTF_Init(), "TTF 初始化失败: ", TTF_Quit, TTF_GetError);

    game_fonts_initialize(fontsize);
    atexit(game_fonts_destroy);
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
        0, "定时器创建失败: ", SDL_GetError);

    return timer;
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
        std::cerr << "无法加载字体 '" << face << "': " << TTF_GetError() << std::endl;
    }

    return font;
}

void WarGrey::STEM::game_font_destroy(TTF_Font* font) {
    TTF_CloseFont(font);
}

/*************************************************************************************************/
WarGrey::STEM::Universe::Universe(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,
        const char *title, SDL_BlendMode bmode, uint32_t fgc, uint32_t bgc)
    : _fgc(fgc), _bgc(bgc) {
    SDL_SetWindowTitle(window, title);                 // 设置标题
    SDL_SetRenderDrawBlendMode(renderer, bmode);
    game_world_reset(renderer, texture, this->_fgc, this->_bgc);
}

void WarGrey::STEM::Universe::on_elapse(SDL_Renderer* renderer, timer_frame_t &frame) {
    // TODO: why some first frames are lost, why some frames duplicate.
    // printf("%u\t%u\n", frame.count, frame.uptime);
    
    game_world_reset(renderer, this->_fgc, this->_bgc);
    this->update(renderer, frame.interval, frame.count, frame.uptime);
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
    if (keyboard.state == SDL_RELEASED) {
        SDL_Keysym key = keyboard.keysym;

        this->on_char(key.sym, key.mod, keyboard.repeat);
    }
}

