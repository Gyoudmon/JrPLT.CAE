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

#define Game_Close_Font(id) if (id != NULL) TTF_CloseFont(id); id == NULL

/*************************************************************************************************/
static std::unordered_map<std::string, std::string> system_fonts;
static std::string system_fontdirs[] = {
    "/System/Library/Fonts",
    "/Library/Fonts",
    "C:\\Windows\\Fonts",
    "/usr/share/fonts"
};

TTF_Font* WarGrey::STEM::GAME_DEFAULT_FONT = NULL;

static void game_push_fonts_of_directory(std::filesystem::path& root) {
    for (auto entry : directory_iterator(root)) {
        path self = entry.path();

        if (entry.is_directory()) {
            game_push_fonts_of_directory(self);
        } else if (entry.is_regular_file()) {
            system_fonts[self.filename()] = self.string();
        }
    }
}

static void game_fonts_initialize(int fontsize = 24) {
    for (int idx = 0; idx < sizeof(system_fontdirs) / sizeof(std::string); idx++) {
        path root(system_fontdirs[idx]);

        if (exists(root) && is_directory(root)) {
            game_push_fonts_of_directory(root);
        }
    }

    GAME_DEFAULT_FONT = game_create_font("Apple Symbols.ttf", fontsize);
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

