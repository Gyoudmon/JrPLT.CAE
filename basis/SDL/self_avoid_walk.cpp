#include "self_avoid_walk.hpp"

#include "text.hpp"
#include "random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static int width;
static int height;

static const int GRID_SIZE = 16;     // 方格边长
static const int WORLD_SIZE = 48;    // 方格单边数量
static const int WORLD_LENGTH = WORLD_SIZE * GRID_SIZE;

static TTF_Font* MESSAGE_FONT = GAME_DEFAULT_FONT;
static int MESSAGE_OFFSET = 10;
static int lineheight;

static int world_x;
static int world_y;

static SDL_Rect grid_self;
static int grids[WORLD_SIZE + 2][WORLD_SIZE + 2];
static int x;
static int y;

static inline bool is_dead_end(int x, int y) {
    return ((grids[x - 1][y - 1] == 1) && (grids[x - 1][y + 1] == 1)
                && (grids[x + 1] [y - 1] == 1) && (grids[x + 1][y + 1] == 1));
}

static inline bool is_in_world(int x, int y) {
    return ((x > 0) && (x <= WORLD_SIZE)
             && (y > 0) && (y <= WORLD_SIZE));
}

/*************************************************************************************************/
void* self_avoid_walk_initialize(int argc, char* args[], SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetWindowTitle(window, "Self Avoiding Walk");           // 设置标题
    SDL_GetWindowSize(window, &width, &height);                 // 获知窗口大小
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);   // 设置混色模式, 不要混色！！！
    
    // 确保游戏世界被绘制在屏幕中心
    world_x = (width - WORLD_LENGTH) / 2;
    world_y = (height - WORLD_LENGTH) / 2;

    // 初始化世界
    for (int i = 0; i < WORLD_SIZE + 2; i++) {
        for (int j = 0; j < WORLD_SIZE + 2; j++) {
            if ((i == 0) || (j == 0) || (i == WORLD_SIZE + 1) || (j == WORLD_SIZE + 1)) {
                grids[i][j] = 1; // 假设外围有墙
            } else {
                grids[i][j] = 0;
            }
        }
    }

    // 起始位置在世界中心
    x = WORLD_SIZE / 2 + 1;
    y = WORLD_SIZE / 2 + 1;
    grids[x][y] = 1;

    grid_self.w = GRID_SIZE;
    grid_self.h = GRID_SIZE;

    game_text_size(MESSAGE_FONT, NULL, &lineheight, "em");

    return NULL;
}

void update_self_avoid_walk(timer_frame_t* frame, void* datum, SDL_Renderer* renderer) {
    // 绘制游戏世界的网格
    for (int i = 0; i <= WORLD_SIZE; i++) {
        int xi = world_x + i * GRID_SIZE;

        for (int j = 0; j <= WORLD_SIZE; j++) {
            int yj = world_y + j * GRID_SIZE;

            SDL_RenderDrawLine(renderer, world_x, yj, world_x + WORLD_LENGTH, yj);
        }

        SDL_RenderDrawLine(renderer, xi, world_y, xi, world_y + WORLD_LENGTH);
    }

    // 移动, 直到走出地图或走进死胡同
    if (is_in_world(x, y)) {
        if (!is_dead_end(x, y)) {
            int cur_x = x;
            int cur_y = y;

            do {
                x = cur_x;
                y = cur_y;

                switch (random_uniform(0, 3) % 4) {
                    case 0: x -= 1; break;
                    case 1: x += 1; break;
                    case 2: y -= 1; break;
                    case 3: y += 1; break;
                }
            
                game_draw_shaded_text(MESSAGE_FONT, renderer, 0x0000FFFF, 0xFF,
                        MESSAGE_OFFSET, MESSAGE_OFFSET,
                        "Move To (%d, %d)  ", x, y);

            } while ((grids[x][y] == 1) && is_in_world(x, y));
        } else {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            game_draw_solid_text(MESSAGE_FONT, renderer, 0xFF0000FF,
                    MESSAGE_OFFSET, MESSAGE_OFFSET + lineheight,
                    "Failed!");
        }
    } else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        game_draw_solid_text(MESSAGE_FONT, renderer, 0x00FF00FF,
                MESSAGE_OFFSET, MESSAGE_OFFSET + lineheight,
                "Succeed!");
    }

    // 绘制路径
    grids[x][y] = 1;
    for (int i = 0; i < WORLD_SIZE; i++) {
        for (int j = 0; j < WORLD_SIZE; j++) {
            if (grids[i + 1][j + 1] == 1) {
                grid_self.x = world_x + i * GRID_SIZE;
                grid_self.y = world_y + j * GRID_SIZE;
                SDL_RenderFillRect(renderer, &grid_self);
            }
        }
    }
}

