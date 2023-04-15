#include "game_of_life.hpp"

#include "../big_bang/graphics/text.hpp"
#include "../big_bang/graphics/image.hpp"

#include "../big_bang/physics/random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static inline int check_neighbor(int* world[], int row, int col, int r, int c) {
    return ((r >= 0) && (r < row)
            && (c >= 0) && (c < col)
            && (world[r][c] > 0))
        ? 1 : 0;
}

static inline int count_neighbors(int *world[], int row, int col, int r, int c) {
    return check_neighbor(world, row, col, r - 1, c + 0)  // up
         + check_neighbor(world, row, col, r + 1, c + 0)  // down
         + check_neighbor(world, row, col, r + 0, c - 1)  // left
         + check_neighbor(world, row, col, r + 0, c + 1)  // right
         + check_neighbor(world, row, col, r - 1, c - 1)  // left-up
         + check_neighbor(world, row, col, r + 1, c + 1)  // right-down
         + check_neighbor(world, row, col, r + 1, c - 1)  // left-down
         + check_neighbor(world, row, col, r - 1, c + 1); // right-up
}

/*************************************************************************************************/
WarGrey::STEM::GameOfLifelet::GameOfLifelet(int row, int col, float gridsize)
    : row(row), col(col), gridsize(gridsize) {}

WarGrey::STEM::GameOfLifelet::~GameOfLifelet() {
    for (int r = 0; r < this->row; r ++) {
        delete [] this->world[r];
    }

    delete [] this->world;
    delete [] this->shadow;
}

void WarGrey::STEM::GameOfLifelet::construct(SDL_Renderer* renderer) {
    IGraphlet::construct(renderer);

    this->shadow = new int[this->row * this->col];
    this->world = new int*[this->row];

    for (int r = 0; r < this->row; r ++) {
        this->world[r] = new int[this->col];
    }

    this->reset();
}

void WarGrey::STEM::GameOfLifelet::feed_extent(float x, float y, float* width, float* height) {
    SET_BOX(width, this->gridsize * float(this->col) + 1.0F);
    SET_BOX(height, this->gridsize * float(this->row) + 1.0F);
}

void WarGrey::STEM::GameOfLifelet::draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    game_draw_frame(renderer, x, y, Width, Height);

    // 绘制舞台的网格
    if (!this->hide_grid) {
        game_draw_grid(renderer, this->row, this->col, this->gridsize, this->gridsize, x, y);
    }

    // 绘制生命
    game_fill_grid(renderer, this->world, this->row, this->col, this->gridsize, this->gridsize, x, y);
}

void WarGrey::STEM::GameOfLifelet::show_grid(bool yes) {
    if (this->hide_grid == yes) {
        this->hide_grid = !yes;
        this->notify_updated();
    }
}

void WarGrey::STEM::GameOfLifelet::construct_random_world() {
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            this->world[i][j] = ((random_raw() % 2 == 0) ? 1 : 0);
        }
    }
}

void WarGrey::STEM::GameOfLifelet::evolve(int** world, int* shadow, int row, int col) {
    for (int r = 0; r < row; r ++) {
        for (int c = 0; c < col; c ++) {
            int n = count_neighbors(world, row, col, r, c);
            int i = r * row + c;

            if (n < 2) {            // 死亡(离群索居)
                shadow[i] = 0;
            } else if (n > 3) {     // 死亡(过度竞争)
                shadow[i] = 0;
            } else if (n == 3) {    // 无性繁殖
                shadow[i] = 1;
            } else {                // 安居乐业
                shadow[i] = world[r][c];
            }
        }
    }
}

bool WarGrey::STEM::GameOfLifelet::pace_forward(int repeats) {
    bool evolved = false;

    // 应用演化规则
    this->evolve(this->world, this->shadow, this->row, this->col);

    // 同步舞台状态
    for (int r = 0; r < this->row; r ++) {
        for (int c = 0; c < this->col; c ++) {
            int state = this->shadow[r * this->col + c];

            if (this->world[r][c] != state) {
                this->world[r][c] = state;
                evolved = true;
            }
        }
    }

    if (evolved) {
        this->generation ++;
    }

    return evolved;
}

void WarGrey::STEM::GameOfLifelet::reset() {
    this->generation = 0;

    for (int r = 0; r < this->row; r ++) {
        for (int c = 0; c < this->col; c ++) {
            this->world[r][c] = 0;
        }
    }
}

/*************************************************************************************************/
WarGrey::STEM::GameOfLifeWorld::GameOfLifeWorld(float gridsize)
    : GameOfLifeWorld("生命游戏", gridsize) {}

WarGrey::STEM::GameOfLifeWorld::GameOfLifeWorld(const char* title, float gridsize)
    : TheBigBang(title), gridsize(gridsize) {}

WarGrey::STEM::GameOfLifeWorld::~GameOfLifeWorld() {}

/*************************************************************************************************/
void WarGrey::STEM::GameOfLifeWorld::load(float width, float height) {
    int n = fl2fxi(flmin(width, height - generic_font_size(FontSize::xx_large)) / this->gridsize) - 1;

    this->gameboard = this->insert(new GameOfLifelet(n, n, this->gridsize));

    TheBigBang::load(width, height);
    this->switch_game_state(GameState::Run);
}

void WarGrey::STEM::GameOfLifeWorld::reflow(float width, float height) {
    TheBigBang::reflow(width, height);
    this->move_to(this->gameboard, width * 0.5F, height * 0.5F, MatterAnchor::CC);
}

void WarGrey::STEM::GameOfLifeWorld::update(uint64_t count, uint32_t interval, uint64_t uptime) {
}

/*************************************************************************************************/
void WarGrey::STEM::GameOfLifeWorld::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    /*
    if (!pressed) {
        switch(key) {
            case 's': this->switch_game_state(GameState::Stop); break;
            case 'p': this->switch_game_state(GameState::Run); break;
            case 'e': this->switch_game_state(GameState::Edit); break;
            case 'r': this->construct_random_game_world(); break;
            case 'c': this->reset_game_world(); break;
            case 'f': this->forward_game_world(repeats, false); break;
        }

        this->notify_updated();
    }
    */
}

/*
void WarGrey::STEM::GameOfLifeWorld::on_click(int x, int y) {
    if (this->state == GameState::Edit) {
        int sx = x - this->stage_x;
        int sy = y - this->stage_y;
        int mx = this->stage_width * GRID_SIZE;
        int my = this->stage_height * GRID_SIZE;

        if ((sx > 0) && (sx < mx) && (sy > 0) && (sy < my)) {
            int gx = sx / GRID_SIZE;
            int gy = sy / GRID_SIZE;

            if ((gx < this->stage_width) && (gy < this->stage_height)) {
                this->world[gx][gy] = (this->world[gx][gy] == 0) ? 1 : 0;
                this->notify_updated();
            }
        }
    }
}
*/

/*************************************************************************************************/
void WarGrey::STEM::GameOfLifeWorld::switch_game_state(GameState new_state) {
    if (this->state != new_state) {
        switch (new_state) {
        case GameState::Run: {
            this->state = new_state;
        }; break;
        case GameState::Stop: {
            if (this->state == GameState::Run) { 
                this->state = new_state;
            }
        }; break;
        case GameState::Edit: {
            if (this->state == GameState::Stop) {
                this->state = new_state;
            }
        }; break;
        default: /* 什么都不做 */; break;
        }
    }
}

/*
WarGrey::STEM::ConwayLifeWorld::ConwayLifeWorld() : GameOfLifeWorld("Conway's Game of Life") {}

void WarGrey::STEM::ConwayLifeWorld::evolve(int** world, int* shadow, int stage_width, int stage_height) {
    for (int x = 0; x < stage_height; x++) {
        for (int y = 0; y < stage_width; y++) {
            int n = count_neighbors(world, stage_width, stage_height, x, y);
            int i = x * stage_width + y;

            if (n < 2) {            // 死亡(离群索居)
                shadow[i] = 0;
            } else if (n > 3) {     // 死亡(过度竞争)
                shadow[i] = 0;
            } else if (n == 3) {    // 无性繁殖
                shadow[i] = 1;
            } else {                // 安居乐业
                shadow[i] = world[x][y];
            }
        }
    }
}

WarGrey::STEM::HighLifeWorld::HighLifeWorld() : GameOfLifeWorld("High Life") {}

void WarGrey::STEM::HighLifeWorld::evolve(int** world, int* shadow, int stage_width, int stage_height) {
    for (int x = 0; x < stage_height; x++) {
        for (int y = 0; y < stage_width; y++) {
            int n = count_neighbors(world, stage_width, stage_height, x, y);
            int i = x * stage_width + y;

            switch (n) {
            case 3: case 6: shadow[i] = 1; break;
            case 2: shadow[i] = world[x][y]; break;
            default: shadow[i] = 0;
            }
        }
    }
}
*/
