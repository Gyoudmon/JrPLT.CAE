#include "bang.hpp"
#include "splash.hpp"

#include "big_bang/datum/string.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char* unknown_task_name = "冒险越来越深入了";
static const int advent_days = 12;

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
            this->agent->scale(-1.0F, 1.0F);
            
            for (int idx = 0; idx < advent_days; idx ++) {
                const char* task_name = this->master->plane_name(idx + 1);
                
                if (task_name == nullptr) {
                    std::string vname = make_nstring(" %2d %s", idx + 1, unknown_task_name);

                    this->names.push_back(this->insert(new Labellet(bang_font::vertical, GAINSBORO, "%s", vname.c_str())));
                    this->coins.push_back(this->insert(new Coinlet(unknown_task_name, idx + 1)));
                    this->coins.back()->stop();
                } else {
                    std::string vname = make_nstring(" %2d %s", idx + 1, task_name);

                    this->names.push_back(this->insert(new Labellet(bang_font::vertical, ROYALBLUE, "%s", vname.c_str())));
                    this->coins.push_back(this->insert(new Coinlet(task_name, idx + 1)));
                }
            }

            this->tux = this->insert(new Tuxmon());
            this->tux->scale(-1.0F, 1.0F);

            this->sledge->scale(0.80F);
            this->set_background(BLACK);
        }
        
        void reflow(float width, float height) override {
            this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);
            this->move_to(this->sledge, width, 0.0F, MatterAnchor::RT);
            this->move_to(this->splash, width * 0.5F, height * 0.5F, MatterAnchor::CC);
            this->move_to(this->tux, width, 0.0F, MatterAnchor::RT);

            this->create_grid(28, 45, this->splash);
            this->set_grid_color(GRAY);
            
            for (int idx = 0; idx < this->coins.size(); idx ++) {
                if (idx == 0) {
                    this->move_to(this->coins[idx], width, height, MatterAnchor::LB);
                } else {
                    this->move_to(this->coins[idx], this->coins[idx - 1], MatterAnchor::LB, MatterAnchor::LT, 0.0F, 4.0F);
                }

                this->move_to(this->names[idx], this->coins[idx], MatterAnchor::RC, MatterAnchor::LC);
            }
        }

        void update(uint32_t count, uint32_t interval, uint32_t uptime) override {
            if (this->target_plane > 0) {
                if (!this->agent->in_playing()) {
                    this->master->transfer_to_plane(this->target_plane);
                    this->target_plane = 0;
                }
            }
        }

        void on_enter(IPlane* from) override {
            this->agent->play("Greeting", 1);
            this->tux->play("buttjump");
        }

    public:
        bool can_select(WarGrey::STEM::IMatter* m) override {
            return (dynamic_cast<Coinlet*>(m) != nullptr)
                    || (m == this->tux);
        }

        void after_select(IMatter* m, bool yes_or_no) override {
            if (yes_or_no) {
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
        Linkmon* agent;
        Labellet* title;
        std::vector<Coinlet*> coins;
        std::vector<Labellet*> names;
        Sprite* tux;
        GridAtlas* sledge;
        GridAtlas* splash;
        
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
