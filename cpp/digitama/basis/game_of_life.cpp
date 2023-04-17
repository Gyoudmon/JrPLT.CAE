#include "game_of_life.hpp"

#include "../big_bang/graphics/text.hpp"
#include "../big_bang/graphics/image.hpp"

#include "../big_bang/physics/random.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char AUTO_KEY = 'a';
static const char STOP_KEY = 's';
static const char EDIT_KEY = 'e';
static const char RAND_KEY = 'r';
static const char RSET_KEY = 'z';
static const char PACE_KEY = 'p';

static const char ordered_keys[] = { AUTO_KEY, STOP_KEY, EDIT_KEY, RAND_KEY, RSET_KEY, PACE_KEY };
static const uint32_t colors_for_auto[] = { GRAY, GREEN, GRAY, GRAY, GRAY, GRAY };
static const uint32_t colors_for_stop[] = { GREEN, GRAY, GREEN, GRAY, GRAY, GREEN };
static const uint32_t colors_for_edit[] = { GREEN, GRAY, GRAY, GREEN, GREEN, GREEN };

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
WarGrey::STEM::GameOfLifelet::~GameOfLifelet() {
    if (this->world != nullptr) {
        for (int r = 0; r < this->row; r ++) {
            delete [] this->world[r];
        }

        delete [] this->world;
    }

    if (this->shadow != nullptr) {
        delete [] this->shadow;
    }
}

void WarGrey::STEM::GameOfLifelet::construct(SDL_Renderer* renderer) {
    IGraphlet::construct(renderer);

    this->shadow = new int[this->row * this->col];
    this->world = new int*[this->row];

    for (int r = 0; r < this->row; r ++) {
        this->world[r] = new int[this->col];
    }
}

void WarGrey::STEM::GameOfLifelet::feed_extent(float x, float y, float* width, float* height) {
    SET_BOX(width, this->gridsize * float(this->col) + 1.0F);
    SET_BOX(height, this->gridsize * float(this->row) + 1.0F);
}

void WarGrey::STEM::GameOfLifelet::draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    RGB_SetRenderDrawColor(renderer, this->color);

    game_draw_rect(renderer, x, y, Width, Height, this->color);

    // 绘制舞台的网格
    if (!this->hide_grid) {
        game_draw_grid(renderer, this->row, this->col, this->gridsize, this->gridsize, x, y);
    }

    // 绘制生命状态
    game_fill_grid(renderer, this->world, this->row, this->col, this->gridsize, this->gridsize, x, y);
}        

void WarGrey::STEM::GameOfLifelet::modify_life_at_location(float x, float y) {
    int c = fl2fxi(flfloor(x / this->gridsize));
    int r = fl2fxi(flfloor(y / this->gridsize));

    this->world[r][c] = (this->world[r][c] == 0) ? 1 : 0;
    this->notify_updated();
}

void WarGrey::STEM::GameOfLifelet::show_grid(bool yes) {
    if (this->hide_grid == yes) {
        this->hide_grid = !yes;
        this->notify_updated();
    }
}

void WarGrey::STEM::GameOfLifelet::set_color(uint32_t hex) {
    if (this->color != hex) {
        this->color = hex;
        this->notify_updated();
    }
}

bool WarGrey::STEM::GameOfLifelet::pace_forward(int repeats) {
    bool evolved = false;

    do {
        bool self_evolved = false;

        // 应用演化规则
        this->evolve(this->world, this->shadow, this->row, this->col);

        // 同步舞台状态
        for (int r = 0; r < this->row; r ++) {
            for (int c = 0; c < this->col; c ++) {
                int state = this->shadow[r * this->col + c];

                if (this->world[r][c] != state) {
                    this->world[r][c] = state;
                    self_evolved = true;
                }
            }
        }

        if (self_evolved) {
            this->generation ++;
            evolved = self_evolved;
        }

        repeats --;
    } while (repeats > 0);

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

void WarGrey::STEM::GameOfLifelet::construct_random_world() {
    for (int r = 0; r < this->row; r++) {
        for (int c = 0; c < this->col; c++) {
            this->world[r][c] = ((random_raw() % 2 == 0) ? 1 : 0);
        }
    }

    this->generation = 0;
}

void WarGrey::STEM::GameOfLifelet::save(const std::string& life_world, std::ofstream& golout) {
    if (world != nullptr) {
        for (int r = 0; r < this->row; r++) {
            for (int c = 0; c < this->col; c++) {
                golout << this->world[r][c];
            }
            
            golout << std::endl;
        }
    }
}

/*************************************************************************************************/
class ConwayLifelet : public WarGrey::STEM::GameOfLifelet {
    using GameOfLifelet::GameOfLifelet;

protected:
    void evolve(int** world, int* shadow, int row, int col) override {
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
};

class HighLifelet : public WarGrey::STEM::GameOfLifelet {
    using GameOfLifelet::GameOfLifelet;

protected:
    void evolve(int** world, int* shadow, int row, int col) override {
        for (int r = 0; r < row; r ++) {
            for (int c = 0; c < col; c ++) {
                int n = count_neighbors(world, row, col, r, c);
                int i = r * row + c;

                switch (n) {
                case 3: case 6: shadow[i] = 1; break;
                case 2: shadow[i] = world[r][c]; break;
                default: shadow[i] = 0;
                }
            }
        }
    }
};

/*************************************************************************************************/
void WarGrey::STEM::GameOfLifeWorld::load(float width, float height) {
    TheBigBang::load(width, height);

    float board_height = height - this->get_titlebar_height() * 2.0F;
    float board_width = width;
    int n = fl2fxi(flmin(board_width, board_height) / this->gridsize) - 1;

    this->gameboard = this->insert(new ConwayLifelet(n, this->gridsize));
    this->generation = this->insert(new Labellet(GameFont::math(), GREEN, "%d", this->gameboard->current_generation()));

    this->instructions[AUTO_KEY] = this->insert(new Labellet(GameFont::monospace(), "%c. 自行演化", AUTO_KEY));
    this->instructions[STOP_KEY] = this->insert(new Labellet(GameFont::monospace(), "%c. 停止演化", STOP_KEY));
    this->instructions[EDIT_KEY] = this->insert(new Labellet(GameFont::monospace(), "%c. 手动编辑", EDIT_KEY));
    this->instructions[RAND_KEY] = this->insert(new Labellet(GameFont::monospace(), "%c. 随机重建", RAND_KEY));
    this->instructions[RSET_KEY] = this->insert(new Labellet(GameFont::monospace(), "%c. 世界归零", RSET_KEY));
    this->instructions[PACE_KEY] = this->insert(new Labellet(GameFont::monospace(), "%c. 单步跟踪", PACE_KEY));

    this->set_local_fps(10);
}

void WarGrey::STEM::GameOfLifeWorld::reflow(float width, float height) {
    TheBigBang::reflow(width, height);

    this->move_to(this->gameboard, width * 0.5F, (height + this->get_titlebar_height()) * 0.5F, MatterAnchor::CC);
    this->move_to(this->generation, this->gameboard, MatterAnchor::RT, MatterAnchor::RB);

    this->move_to(this->instructions[ordered_keys[0]], 0.0F, height, MatterAnchor::LB);
    for (int idx = 1; idx < sizeof(ordered_keys) / sizeof(char); idx ++) {
        this->move_to(this->instructions[ordered_keys[idx]],
                    this->instructions[ordered_keys[idx - 1]], MatterAnchor::RB,
                    MatterAnchor::LB, 16.0F);
    }
}

void WarGrey::STEM::GameOfLifeWorld::on_mission_start(float width, float height) {
    this->switch_game_state(GameState::Stop);
}

void WarGrey::STEM::GameOfLifeWorld::update(uint64_t count, uint32_t interval, uint64_t uptime) {
    switch (this->state) {
    case GameState::Auto: { this->pace_forward(1); }; break;
    default: /* do nothing */; break;
    }
}

/*************************************************************************************************/
bool WarGrey::STEM::GameOfLifeWorld::can_select(IMatter* m) {
    return m == this->agent
        || ((this->state == GameState::Edit)
                && (m == this->gameboard));  
}

void WarGrey::STEM::GameOfLifeWorld::on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) {
    if (!pressed) {
        if (this->instructions.find(key) != this->instructions.end()) {
            if (this->instructions[key]->get_text_color() == GREEN) {
                switch(key) {
                case AUTO_KEY: this->switch_game_state(GameState::Auto); break;
                case STOP_KEY: this->switch_game_state(GameState::Stop); break;
                case EDIT_KEY: this->switch_game_state(GameState::Edit); break;
                case RAND_KEY: this->agent->play_writing(1); this->gameboard->construct_random_world(); break;
                case RSET_KEY: this->agent->play_empty_trash(1); this->gameboard->reset(); break;
                case PACE_KEY: this->agent->play_processing(1); this->pace_forward(1); break;
                }

                this->notify_updated();
            } else {
                this->instructions[key]->set_text_color(CRIMSON);
            }
        }
    }
}

void WarGrey::STEM::GameOfLifeWorld::on_tap(IMatter* matter, float x, float y) {
    if (this->state == GameState::Edit) {
        this->gameboard->modify_life_at_location(x, y);
    }
}

void WarGrey::STEM::GameOfLifeWorld::on_save(const std::string& life_world, std::ofstream& golout) {
    this->gameboard->save(life_world, golout);
}

/*************************************************************************************************/
void WarGrey::STEM::GameOfLifeWorld::pace_forward(int repeats) {
    if (this->gameboard->pace_forward(1)) {
        this->notify_updated();
        this->generation->set_text_color(GREEN);
        this->generation->set_text(MatterAnchor::RB, "%d", this->gameboard->current_generation());
    } else {
        this->generation->set_text_color(CRIMSON);
        this->generation->set_text(MatterAnchor::RB, "%d", this->gameboard->current_generation());
    }
}

/*************************************************************************************************/
void WarGrey::STEM::GameOfLifeWorld::switch_game_state(GameState new_state) {
    if (this->state != new_state) {
        switch (new_state) {
        case GameState::Auto: {
            this->gameboard->set_color(LIGHTSKYBLUE);
            this->gameboard->show_grid(false);
            this->agent->play_thinking(8);
            this->update_instructions_state(colors_for_auto);
        }; break;
        case GameState::Stop: {
            this->gameboard->set_color(BLACK);
            this->agent->play_rest_pose(1);
            this->update_instructions_state(colors_for_stop);
        }; break;
        case GameState::Edit: {
            this->gameboard->set_color(ROYALBLUE);
            this->gameboard->show_grid(true);
            this->agent->play_writing(-1);
            this->update_instructions_state(colors_for_edit);
        }; break;
        default: /* 什么都不做 */; break;
        }

        this->state = new_state;
        this->notify_updated();
    }
}

void WarGrey::STEM::GameOfLifeWorld::update_instructions_state(const uint32_t* colors) {
    for (size_t idx = 0; idx < sizeof(ordered_keys) / sizeof(char);  idx ++) {
        this->instructions[ordered_keys[idx]]->set_text_color(colors[idx]);
    }
}
