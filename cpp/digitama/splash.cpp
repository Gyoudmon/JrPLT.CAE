#include "bang.hpp"
#include "splash.hpp"

#include "big_bang/datum/string.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char* label_fmt = " %2d %s";
static const char* unknown_task_name = "冒险越来越深入了";

static const float tux_speed_walk_x = 2.4F;
static const float tux_speed_jump_x = tux_speed_walk_x;
static const float tux_speed_jump_y = -12.0F;
static const float tux_speed_dy = 1.0F;

static const std::vector<std::pair<int, int>> tux_spots = {
    /* { row, col } */
    { 6, 4 },
    { 6, 15 }, { 12, 24 },
    { 17, 34 }, { 22, 44 }
};

static const std::vector<std::pair<int, int>> coin_positions = {
    /* { row, col } */
    { 5, 9 }, { 11, 18 }, { 16, 27 }, { 21, 37 }
};

/*************************************************************************************************/
namespace {
    class BigBangPlane : public Plane {
    public:
        BigBangPlane(Cosmos* master) : Plane("Big Bang!"), master(master) {}

    public:  // 覆盖游戏基本方法
        void load(float width, float height) override {
            this->sledge = this->insert(new GridAtlas("sledge.png"));
            this->splash = this->insert(new GridAtlas("splash.png"));
            this->title = this->insert(new Labellet(bang_font::title, GHOSTWHITE, title_fmt, "宇宙大爆炸"));
            this->agent = this->insert(new Linkmon());
            
            for (int idx = 0; idx < coin_positions.size(); idx ++) {
                const char* task_name = this->master->plane_name(idx + 1);
                int index = idx + 1;
                
                if (task_name == nullptr) {
                    this->names.push_back(this->insert(new Labellet(bang_font::tiny, GAINSBORO, label_fmt, index, unknown_task_name)));
                    this->coins.push_back(this->insert(new Coinlet(unknown_task_name, index)));
                    this->coins.back()->stop();
                } else {
                    this->names.push_back(this->insert(new Labellet(bang_font::tiny, ROYALBLUE, label_fmt, index, task_name)));
                    this->coins.push_back(this->insert(new Coinlet(task_name, index)));
                }
            }

            this->tux = this->insert(new Tuxmon());

            this->agent->scale(-1.0F, 1.0F);
            this->sledge->scale(0.80F);
            this->splash->create_logic_grid(28, 45);
            // this->splash->set_logic_grid_color(DIMGRAY);

            this->set_background(BLACK);
        }
        
        void reflow(float width, float height) override {
            float dx, dy;

            this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);
            this->move_to(this->sledge, width, 0.0F, MatterAnchor::RT);
            this->move_to(this->splash, width * 0.5F, height * 0.5F, MatterAnchor::CC);
            
            this->tux_home();
            
            for (int idx = 0; idx < coin_positions.size(); idx ++) {
                auto pos = coin_positions[idx];

                this->splash->feed_logic_tile_location(pos.first, pos.second, &dx, &dy, MatterAnchor::CC, false);

                this->move_to(this->coins[idx], dx, dy, MatterAnchor::CC);
                this->move_to(this->names[idx], this->coins[idx], MatterAnchor::CB, MatterAnchor::CT);
                this->names[idx]->show(false);
            }
        }

        void update(uint32_t count, uint32_t interval, uint32_t uptime) override {
            this->tux_step(count, interval, uptime);

            if (this->target_plane > 0) {
                if (!this->agent->in_playing()) {
                    this->master->transfer_to_plane(this->target_plane);
                    this->target_plane = 0;
                }
            }
        }

        void on_enter(IPlane* from) override {
            this->agent->play("Greeting", 1);
            this->tux->set_speed(tux_speed_walk_x, 0.0F);
        }

    public:
        bool can_select(WarGrey::STEM::IMatter* m) override {
            return (dynamic_cast<Coinlet*>(m) != nullptr)
                    || (m == this->tux);
        }

        void after_select(IMatter* m, bool yes) override {
            if (yes) {
                if (m == this->tux) {
                    if (this->tux->is_wearing()) {
                        this->tux->take_off();
                    } else {
                        this->tux->wear("ice_hat");
                    }
                } else {
                    Coinlet* coin = dynamic_cast<Coinlet*>(m);

                    if (coin != nullptr) {
                        this->target_plane = coin->idx;
                        this->agent->play("Hide", 1);
                    }
                }
            }
        }
    
    private:
        void tux_home() {
            int row = tux_spots[0].first;
            int col = tux_spots[0].second;
            float x0, y0;
            
            this->splash->feed_logic_tile_location(row, col, &x0, &y0, MatterAnchor::LB, false);
            this->move_to(this->tux, x0, y0, MatterAnchor::CB);
            this->tux_walk_segment = 1;
            this->tux_start_walk();
        }

        void tux_start_walk() {
            this->tux->play("walk");
            this->tux->set_speed_xy(tux_speed_walk_x, 0.0F);
            this->tux_target_y = 0.0F;
        }

        void tux_step(uint32_t count, uint32_t interval, uint32_t uptime) {
            float x0, y0, tx, ty, gx;
            
            this->feed_matter_location(this->tux, &tx, &ty, MatterAnchor::RB);
            this->feed_matter_location(this->splash, &x0, &y0, MatterAnchor::LT);
            
            tx -= x0;
            ty -= y0;

            if (this->tux_target_y == 0.0F) {
                this->feed_splash_location(this->tux_walk_segment, &gx, nullptr);

                if (tx >= gx) {
                    this->tux_walk_segment += 1;
                    if (this->tux_walk_segment < tux_spots.size()) {
                        this->feed_splash_location(this->tux_walk_segment, nullptr, &this->tux_target_y);
                        this->tux->play("buttjump");
                        this->tux->set_speed_xy(tux_speed_jump_x, tux_speed_jump_y);
                    } else {
                        this->tux_home();
                    }
                }
            } else if (ty >= this->tux_target_y) {
                this->tux_start_walk();
            } else {
                this->tux->set_speed_xy(tux_speed_jump_x, this->tux->y_speed() + tux_speed_dy);
            }
        }

        void feed_splash_location(size_t idx, float* x, float* y) {
            this->splash->feed_logic_tile_location(
                tux_spots[idx].first,
                tux_spots[idx].second,
                x, y,
                MatterAnchor::LB);
        }

    private:
        Linkmon* agent;
        Labellet* title;
        std::vector<Coinlet*> coins;
        std::vector<Labellet*> names;
        Sprite* tux;
        GridAtlas* sledge;
        GridAtlas* splash;

    private:
        int tux_walk_segment = 1;
        float tux_target_y = 0.0F;
        
    private:
        Cosmos* master;
        int target_plane = 0;
    };
}

/*************************************************************************************************/
WarGrey::STEM::TheCosmos::~TheCosmos() {
    bang_fonts_destroy();
    imgdb_teardown();
}

void WarGrey::STEM::TheCosmos::construct(int argc, char* argv[]) {
    enter_digimon_zone(argv[0]);
    bang_fonts_initialize();
    imgdb_setup(digimon_zonedir().append("stone"));
    this->parse_cmdline_options(argc, argv);

    this->push_plane(new BigBangPlane(this));
}

void WarGrey::STEM::TheCosmos::update(uint32_t count, uint32_t interval, uint32_t uptime) {
    if (this->has_current_mission_completed()) {
        this->transfer_to_plane(0);
    }
}
