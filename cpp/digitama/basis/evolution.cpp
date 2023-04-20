#include "evolution.hpp"

#include "../big_bang/datum/fixnum.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const char* day_fmt = "Day: %d";

/*************************************************************************************************/
void WarGrey::STEM::EvolutionWorld::load(float width, float height) {
    TheBigBang::load(width, height);
    
    float world_width = width;
    float world_height = height - this->get_titlebar_height();
    float logic_width;
    
    this->col = fl2fxi(world_width / this->size_hint) - 1;
    this->row = fl2fxi(world_height / this->size_hint) - 1;

    // 初始化世界
    this->steppe = this->insert(new SteppeAtlas(this->row, this->col));
    this->day_info = this->insert(new Labellet(GameFont::math(), BLACK, day_fmt, 0));

    this->animals.push_back(this->insert(new TMRooster(this->row, this->col)));
    this->animals.push_back(this->insert(new TMCow(this->row, this->col)));
    this->animals.push_back(this->insert(new TMCat(this->row, this->col)));

    /* 简单配置物体 */
    this->steppe->feed_logic_tile_extent(&logic_width);
    this->steppe->scale_to(this->size_hint / logic_width);
}

void WarGrey::STEM::EvolutionWorld::reflow(float width, float height) {
    TheBigBang::reflow(width, height);
    float cx = width * 0.5F;
    float cy = height * 0.5F;
    
    this->move_to(this->steppe, cx, cy, MatterAnchor::CC);
    this->move_to(this->day_info, this->steppe, MatterAnchor::RT, MatterAnchor::RB);
}

void WarGrey::STEM::EvolutionWorld::on_mission_start(float width, float height) {
    float bottom_overlay;

    this->steppe->feed_map_overlay(nullptr, nullptr, &bottom_overlay);
    
    this->reset_world();
    for (auto animal : this->animals) {
        auto self = animal->unsafe_metadata<IToroidalMovingAnimal>();

        this->steppe->move_to_logic_tile(animal,
            self->current_row(), self->current_col(), MatterAnchor::CC,
            MatterAnchor::CB, 0.0F, bottom_overlay);
    }
}

void WarGrey::STEM::EvolutionWorld::update(uint64_t count, uint32_t interval, uint64_t uptime) {
    if (this->animals.empty()) {
        this->day_info->set_text_color(FIREBRICK);
    } else {
        float tile_width, tile_height;
        bool has_death = false;
        
        this->steppe->feed_logic_tile_extent(&tile_width, &tile_height);

        for (auto animal : this->animals) {        
            if (animal->motion_stopped()) {
                auto self = animal->unsafe_metadata<IToroidalMovingAnimal>();
    
                if (self->is_alive()) {
                    this->animal_live(animal, self, tile_width, tile_height);
                } else {
                    has_death = true;
                }
            }
        }

        if (has_death) {
            this->clear_dead_animals();
        }
    }

    this->update_dayinfo();
}

void WarGrey::STEM::EvolutionWorld::animal_live(Animal* animal, IToroidalMovingAnimal* self, float tile_width, float tile_height) {
    int dr, dc;

    self->turn();
    self->move(&dr, &dc);

    if ((fxabs(dr) > 1) || (fxabs(dc) > 1)) {
        this->move(animal, dc * tile_width, dr * tile_height);
    } else {
        this->glide(self->pace_duration(), animal, dc * tile_width, dr * tile_height);
    }
}

void WarGrey::STEM::EvolutionWorld::clear_dead_animals() {
    std::for_each(this->animals.begin(), this->animals.end(),
        [&, this](Animal*& animal) {
            auto self = animal->unsafe_metadata<IToroidalMovingAnimal>();
            
            if (!self->is_alive()) {
                this->remove(animal);
                animal = nullptr;
            }
        });

    auto it = std::remove(this->animals.begin(), this->animals.end(), static_cast<Animal*>(nullptr));
    this->animals.erase(it, this->animals.end());
}

/**************************************************************************************************/
bool WarGrey::STEM::EvolutionWorld::can_select(IMatter* m) {
    return (m == this->agent) || isinstance(m, Animal);
}

void WarGrey::STEM::EvolutionWorld::after_select(IMatter* m, bool yes) {
}

void WarGrey::STEM::EvolutionWorld::reset_world() {
    this->steppe->reset();
    this->day_info->set_text_color(FORESTGREEN);
    this->update_dayinfo();
}

/**************************************************************************************************/
bool WarGrey::STEM::EvolutionWorld::update_tooltip(IMatter* m, float lx, float ly, float gx, float gy) {
    bool updated = false;

    return updated;
}

void WarGrey::STEM::EvolutionWorld::update_dayinfo() {
    this->day_info->set_text(MatterAnchor::RB, day_fmt, this->steppe->current_day());
}
