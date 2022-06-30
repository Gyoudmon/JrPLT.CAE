#include "game_of_life.hpp"

#include "text.hpp"
#include "random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const int GRID_SIZE = 16;     // 方格边长
static const int WORLD_SIZE = 48;    // 方格单边数量

static inline int check_neighbor(int* world[], int nx, int ny, int x, int y) {
    return ((x >=0) && (x < nx)
            && (y >= 0) && (y < ny)
            && (world[x][y] > 0))
        ? 1 : 0;
}

static inline int count_neighbors(int *world[], int nx, int ny, int x, int y) {
    return check_neighbor(world, nx, ny, x - 1, y + 0)  // left
         + check_neighbor(world, nx, ny, x + 1, y + 0)  // right
         + check_neighbor(world, nx, ny, x + 0, y - 1)  // up
         + check_neighbor(world, nx, ny, x + 0, y + 1)  // down
         + check_neighbor(world, nx, ny, x - 1, y - 1)  // left-up
         + check_neighbor(world, nx, ny, x + 1, y + 1)  // right-down
         + check_neighbor(world, nx, ny, x + 1, y - 1)  // right-up
         + check_neighbor(world, nx, ny, x - 1, y + 1); // left-down
}

/*************************************************************************************************/
WarGrey::STEM::GameOfLife::GameOfLife(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, const char* title)
    : Universe(window, renderer, texture, title, SDL_BLENDMODE_NONE, 0xFFFFFFFF, 0x000000FF) {}

WarGrey::STEM::GameOfLife::~GameOfLife() {
    /* 销毁世界 */ {
        for (int i = 0; i < this->world_height; i++) {
            delete [] this->world[i];
        }

        delete [] this->world;
    }

    delete [] this->shadow;
}

/*************************************************************************************************/
void WarGrey::STEM::GameOfLife::construct(int argc, char* argv[], int width, int height) {
    this->world_width = WORLD_SIZE;
    this->world_height = WORLD_SIZE;

    // 确保游戏世界被绘制在屏幕中心
    this->draw_x = (width - this->world_width * GRID_SIZE) / 2;
    this->draw_y = (height - this->world_height * GRID_SIZE) / 2;

    /* 创建世界网格 */ {
        this->shadow = new int[this->world_width * this->world_height];
        this->world = new int*[this->world_height];

        for (int i = 0; i < this->world_height; i++) {
            this->world[i] = new int[this->world_width];

            for (int j = 0; j < this->world_width; j++) {
                this->world[i][j] = ((random_raw() % 2 == 0) ? 1 : 0);
            }
        }
    }

    game_text_size(game_monospace_font, NULL, &this->lineheight, "em");
    this->generation = 0;
    
    this->set_fps(8);
}

void WarGrey::STEM::GameOfLife::update(SDL_Renderer* renderer, uint32_t interval, uint32_t count, uint32_t uptime) {
    // 绘制游戏边界
    game_draw_frame(renderer, this->draw_x, this->draw_y, this->world_width * GRID_SIZE, this->world_height * GRID_SIZE);

    // 绘制游戏世界的网格
    game_draw_grid(renderer, this->world_width, this->world_height, GRID_SIZE, this->draw_x, this->draw_y);

    // 时间飞逝, 更新网格
    this->timeline_forward(interval, count, uptime);
    game_draw_blended_text(game_monospace_font, renderer, this->get_foreground_color(),
            0, 0, "Generation: %d", this->generation);

    // 绘制生命
    game_fill_grid(renderer, this->world, this->world_width, this->world_height, GRID_SIZE, this->draw_x, this->draw_y);
}

/*************************************************************************************************/
void WarGrey::STEM::GameOfLife::timeline_forward(uint32_t interval, uint32_t count, uint32_t uptime) {
    bool evolved = false;

    // 应用演化规则
    this->evolve(this->world, this->shadow, this->world_width, this->world_height);

    // 同步世界状态
    for (int x = 0; x < this->world_height; x++) {
        for (int y = 0; y < this->world_width; y++) {
            int state = this->shadow[x * this->world_width + y];

            if (this->world[x][y] != state) {
                this->world[x][y] = state;
                evolved = true;
            }
        }
    }

    if (evolved) {
        this->generation ++;
    }
}

/*************************************************************************************************/
WarGrey::STEM::ConwayLife::ConwayLife(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture)
    : GameOfLife(window, renderer, texture, "Conway's Game of Life") {}

void WarGrey::STEM::ConwayLife::evolve(int** world, int* shadow, int world_width, int world_height) {
    for (int x = 0; x < world_height; x++) {
        for (int y = 0; y < world_width; y++) {
            int n = count_neighbors(world, world_width, world_height, x, y);
            int i = x * world_width + y;

            if (n < 2) {            // 死亡(离群索居)
                shadow[i] = 0;
            } else if (n > 3) {     // 死亡(过度竞争)
                shadow[i] = 0;
            } else if (n == 3) {    // 繁殖
                shadow[i] = 1;
            } else {                // 安居乐业
                shadow[i] = world[x][y];
            }
        }
    }
}

/*************************************************************************************************/
WarGrey::STEM::HighLife::HighLife(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture)
    : GameOfLife(window, renderer, texture, "High Life") {}

void WarGrey::STEM::HighLife::evolve(int** world, int* shadow, int world_width, int world_height) {
    for (int x = 0; x < world_height; x++) {
        for (int y = 0; y < world_width; y++) {
            int n = count_neighbors(world, world_width, world_height, x, y);
            int i = x * world_width + y;

            switch (n) {
            case 3: case 6: shadow[i] = 1; break;
            case 2: shadow[i] = world[x][y]; break;
            default: shadow[i] = 0;
            }
        }
    }
}

