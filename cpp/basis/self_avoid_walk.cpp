#include "self_avoid_walk.hpp"

#include "digitama/text.hpp"
#include "digitama/random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const int GRID_SIZE = 16;     // 方格边长
static const int WORLD_LENGTH = WORLD_SIZE * GRID_SIZE;

static inline bool direction_okay(int world[WORLD_SIZE + 2][WORLD_SIZE + 2], int x, int y) {
    return ((x >=0) && (x < (WORLD_SIZE + 2))
            && (y >= 0) && (y < (WORLD_SIZE + 2))
            && (world[x][y] == 0));
}

static inline bool is_dead_end(int world[WORLD_SIZE + 2][WORLD_SIZE + 2], int x, int y) {
    return !direction_okay(world, x - 1, y + 0)  // left
        && !direction_okay(world, x + 1, y + 0)  // right
        && !direction_okay(world, x + 0, y - 1)  // up
        && !direction_okay(world, x + 0, y + 1); // down
}

static inline bool is_inside_world(int x, int y) {
    return ((x > 0) && (x <= WORLD_SIZE)
             && (y > 0) && (y <= WORLD_SIZE));
}

/*************************************************************************************************/
WarGrey::STEM::SelfAvoidWalk::SelfAvoidWalk() : Universe("Self Avoid Walk", 24, 0x000000U, 0xFFFFFFU) {
    game_text_size(game_font::monospace, NULL, &this->lineheight, "em");
}

void WarGrey::STEM::SelfAvoidWalk::construct(int argc, char* args[]) {
    // 初始化世界
    for (int i = 0; i < WORLD_SIZE + 2; i++) {
        for (int j = 0; j < WORLD_SIZE + 2; j++) {
            if ((i == 0) || (j == 0) || (i == WORLD_SIZE + 1) || (j == WORLD_SIZE + 1)) {
                this->grids[i][j] = 1; // 假设外围有墙
            } else {
                this->grids[i][j] = 0;
            }
        }
    }

    // 起始位置在世界中心
    this->x = WORLD_SIZE / 2 + 1;
    this->y = WORLD_SIZE / 2 + 1;
    this->grids[x][y] = 1;
    
    this->state = WalkState::Run;
}

void WarGrey::STEM::SelfAvoidWalk::reflow(int width, int height) {
    // 确保游戏世界被绘制在屏幕中心
    this->world_x = (width - WORLD_LENGTH) / 2;
    this->world_y = (height - WORLD_LENGTH) / 2;
    
    this->grid_self.w = GRID_SIZE;
    this->grid_self.h = GRID_SIZE;
}

void WarGrey::STEM::SelfAvoidWalk::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    if (this->state == WalkState::Run) {
        // 移动, 直到走出地图或走进死胡同
        if (is_inside_world(this->x, this->y)) {
            if (!is_dead_end(this->grids, this->x, this->y)) {
                int cur_x = this->x;
                int cur_y = this->y;

                do {
                    this->x = cur_x;
                    this->y = cur_y;

                    switch (random_uniform(0, 3) % 4) {
                        case 0: this->x -= 1; break;
                        case 1: this->x += 1; break;
                        case 2: this->y -= 1; break;
                        case 3: this->y += 1; break;
                    }
                } while ((this->grids[x][y] == 1) && is_inside_world(this->x, this->y));
            } else {
                this->state = WalkState::Failed;
            }
        } else {
            this->state = WalkState::Succeed;
        }

        // 绘制路径
        this->grids[x][y] = 1;
    }
}

void WarGrey::STEM::SelfAvoidWalk::draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
    // 绘制游戏世界的网格
    for (int i = 0; i <= WORLD_SIZE; i++) {
        int xi = this->world_x + i * GRID_SIZE;

        for (int j = 0; j <= WORLD_SIZE; j++) {
            int yj = this->world_y + j * GRID_SIZE;

            SDL_RenderDrawLine(renderer, this->world_x, yj, this->world_x + WORLD_LENGTH, yj);
        }

        SDL_RenderDrawLine(renderer, xi, this->world_y, xi, this->world_y + WORLD_LENGTH);
    }

    switch (this->state) {
        case WalkState::Run: {
            game_draw_lcd_text(game_font::monospace, renderer, 0xFFA500FF, 0xFF,
                    this->lineheight / 2, this->lineheight / 2,
                    "Move to (%d, %d)  ", this->x, this->y);
        }; break;
        case WalkState::Succeed: {
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            game_draw_blended_text(game_font::sans_serif, renderer, 0x00FF00FF,
                    this->lineheight / 2, this->lineheight * 3 / 2,
                    "Succeed!");

        }; break;
        case WalkState::Failed: {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            game_draw_blended_text(game_font::sans_serif, renderer, 0xFF0000FF,
                    this->lineheight / 2, this->lineheight * 3 / 2,
                    "Failed!");
        }; break;
    }

    // 绘制路径
    for (int i = 0; i < WORLD_SIZE; i++) {
        for (int j = 0; j < WORLD_SIZE; j++) {
            if (this->grids[i + 1][j + 1] == 1) {
                this->grid_self.x = world_x + i * GRID_SIZE;
                this->grid_self.y = world_y + j * GRID_SIZE;
                SDL_RenderFillRect(renderer, &this->grid_self);
            }
        }
    }
}

