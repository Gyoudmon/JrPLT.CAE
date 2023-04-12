#include "self_avoid_walk.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const double gliding_duration = 0.5;

static const GroundBlockType maze_ground_type = GroundBlockType::Dirt;
static const GroundBlockType maze_path_type = GroundBlockType::Soil;
static const GroundBlockType maze_wall_type = GroundBlockType::Grass;

/*************************************************************************************************/
static inline bool direction_okay(int maze[GRID_SIZE + 2][GRID_SIZE + 2], int x, int y) {
    return ((x >=0) && (x < (GRID_SIZE + 2))
            && (y >= 0) && (y < (GRID_SIZE + 2))
            && (maze[x][y] == 0));
}

static inline bool is_dead_end(int maze[GRID_SIZE + 2][GRID_SIZE + 2], int x, int y) {
    return !direction_okay(maze, x - 1, y + 0)  // left
        && !direction_okay(maze, x + 1, y + 0)  // right
        && !direction_okay(maze, x + 0, y - 1)  // up
        && !direction_okay(maze, x + 0, y + 1); // down
}

static inline bool is_inside_maze(int x, int y) {
    return ((x > 0) && (x <= GRID_SIZE)
             && (y > 0) && (y <= GRID_SIZE));
}

static void pace(int maze[GRID_SIZE + 2][GRID_SIZE + 2], int& x, int& y) {
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
    } while ((maze[x][y] == 1) && is_inside_maze(x, y));
}

/*************************************************************************************************/
void WarGrey::STEM::SelfAvoidWalkWorld::load(float width, float height) {
    float cx = width * 0.5F;
    float cy = height * 0.5F;

    // 初始化世界
    for (int i = 0; i < GRID_SIZE + 2; i++) {
        for (int j = 0; j < GRID_SIZE + 2; j++) {
            if (is_inside_maze(i, j)) {
                this->grids[i][j] = 0;
                this->grounds[i][j] = this->insert(new PlanetCuteTile(maze_ground_type), cx, cy, MatterAnchor::CC);
            } else {
                this->grids[i][j] = 1; // 假设外围有墙
                this->grounds[i][j] = this->insert(new PlanetCuteTile(maze_wall_type), cx, cy, MatterAnchor::CC);
            }
        }
    }

    // 添加漫步者
    this->walkers[0] = this->insert(new Estelle());
    this->walkers[1] = this->insert(new Joshua());
    this->walkers[2] = this->insert(new Scherazard());
    this->walkers[3] = this->insert(new Olivier());
    this->walkers[4] = this->insert(new Agate());
    this->walkers[5] = this->insert(new Klose());
    this->walkers[6] = this->insert(new Tita());
    this->walkers[7] = this->insert(new Zin());

    /* locating */ {
        float top, bottom;

        this->grounds[0][0]->feed_extent(0.0F, 0.0F, &grid_width, &grid_height);
        this->grounds[0][0]->feed_margin(0.0F, 0.0F, &top);
        bottom = this->grounds[0][0]->get_thickness();

        this->grid_width -= 1.0F;
        this->grid_height = this->grid_height - top - bottom;
    }

    TheBigBang::load(width, height);
}

void WarGrey::STEM::SelfAvoidWalkWorld::reflow(float width, float height) {
    size_t walker_count = sizeof(this->walkers) / sizeof(Bracer*);
    float y0;

    TheBigBang::reflow(width, height);
    this->feed_matter_location(this->agent, nullptr, &y0, MatterAnchor::LB);
    this->create_grid(int(walker_count), 1, 16.0F, y0, 64.0F, float(GRID_SIZE + 2) * this->grid_height);

    // 确保游戏世界被绘制在屏幕中心(除标题意外的区域)
    this->world_x = flround((width - float(GRID_SIZE + 2) * this->grid_width) * 0.5F);
    this->world_y = flround(((height - y0) - float(GRID_SIZE + 2) * this->grid_height) * 0.5F + y0);

    for (int i = 0; i < GRID_SIZE + 2; i++) {
        for (int j = 0; j < GRID_SIZE + 2; j++) {
            float dx = this->world_x + float(i + 1) * this->grid_width;
            float dy = this->world_y + float(j + 1) * this->grid_height;

            this->glide_to(gliding_duration, this->grounds[i][j], dx, dy, MatterAnchor::RB);
        }
    }
}

void WarGrey::STEM::SelfAvoidWalkWorld::on_mission_start(float width, float height) {
    this->reset_walkers(false);
    this->reset_maze();
}

void WarGrey::STEM::SelfAvoidWalkWorld::update(uint64_t count, uint32_t interval, uint64_t uptime) {
    if (this->current_walker != nullptr) {
        if (this->current_walker->current_mode() == BracerMode::Run) {
            if (this->current_walker->motion_stopped()) {
                // 移动, 直到走出地图或走进死胡同
                if (is_inside_maze(this->x, this->y)) {
                    if (!is_dead_end(this->grids, this->x, this->y)) {
                        int cur_x = this->x;
                        int cur_y = this->y;

                        pace(this->grids, this->x, this->y);
                        this->grids[this->x][this->y] = 1;
                        this->glide(gliding_duration, this->current_walker,
                                        float(this->x - cur_x) * this->grid_width,
                                        float(this->y - cur_y) * this->grid_height);
                    } else {
                        this->current_walker->switch_mode(BracerMode::Lose);
                    }
                } else {
                    this->current_walker->switch_mode(BracerMode::Win, 1);
                }
            } else {
                if (this->is_colliding(this->grounds[this->x][this->y], this->current_walker, MatterAnchor::CC)) {
                    if (is_inside_maze(this->x, this->y)) {
                        this->grounds[this->x][this->y]->set_type(maze_path_type);
                    }
                }
            }
        } else if (!this->current_walker->in_playing()) {
            this->current_walker = nullptr;
        }
    }
}

/**************************************************************************************************/
bool WarGrey::STEM::SelfAvoidWalkWorld::can_select(IMatter* m) {
    return (this->current_walker == nullptr)
            || (m == this->agent);
}

void WarGrey::STEM::SelfAvoidWalkWorld::after_select(IMatter* m, bool yes) {
    if (yes) {
        Bracer* bracer = dynamic_cast<Bracer*>(m);

        if (bracer != nullptr) {
            this->x = GRID_SIZE / 2 + 1;
            this->y = GRID_SIZE / 2 + 1;

            this->current_walker = bracer;
            this->current_walker->switch_mode(BracerMode::Run);
            this->reset_maze();

            this->grounds[this->x][this->y]->set_type(maze_path_type);
            this->grids[this->x][this->y] = 1;
            
            this->move_to(this->current_walker,
                            this->grounds[this->x][this->y], MatterAnchor::CC,
                            MatterAnchor::CC, 0.0F, -this->grounds[this->x][this->y]->get_thickness());
            
            this->no_selected();
        }
    }
}

void WarGrey::STEM::SelfAvoidWalkWorld::reset_walkers(bool keep_mode) {
    size_t walker_count = sizeof(this->walkers) / sizeof(Bracer*);
    
    for (size_t idx = 0; idx < walker_count; idx++) {
        this->move_to_grid(this->walkers[idx], idx, 0, MatterAnchor::CB);
        this->walkers[idx]->set_heading(90.0);

        if (!keep_mode) {
            this->walkers[idx]->switch_mode(BracerMode::Walk);
        }
    }

    this->current_walker = nullptr;
}

void WarGrey::STEM::SelfAvoidWalkWorld::reset_maze() {
    for (int i = 0; i < GRID_SIZE + 2; i++) {
        for (int j = 0; j < GRID_SIZE + 2; j++) {
            if (is_inside_maze(i, j)) {
                this->grids[i][j] = 0;
                this->grounds[i][j]->set_type(maze_ground_type);
            } else {
                this->grids[i][j] = 1; // 假设外围有墙
                this->grounds[i][j]->set_type(maze_wall_type);
            }
        }
    }
}

/**************************************************************************************************/
bool WarGrey::STEM::SelfAvoidWalkWorld::update_tooltip(IMatter* m, float lx, float ly, float gx, float gy) {
    bool updated = false;
    auto bracer = dynamic_cast<Bracer*>(m);

    if (bracer != nullptr) {
        this->tooltip->set_text(" %s ", bracer->name());
        updated = true;
    }

    return updated;
}
