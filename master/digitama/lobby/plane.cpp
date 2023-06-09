#include "plane.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
void WarGrey::STEM::LobbyPlane::load(float width, float height) {
    this->title = this->insert(new Labellet(GameFont::Title(), BLACK, this->name()));
            
    this->agent = this->insert(new Linkmon());
    this->set_sentry_sprite(this->agent);
    this->agent->scale(-1.0F, 1.0F);

    this->tooltip = this->insert(make_label_for_tooltip(GameFont::Tooltip()));
    this->set_tooltip_matter(this->tooltip);
}
        
void WarGrey::STEM::LobbyPlane::reflow(float width, float height) {
    this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);
}

void WarGrey::STEM::LobbyPlane::update(uint64_t count, uint32_t interval, uint64_t uptime) {
}

bool WarGrey::STEM::LobbyPlane::update_tooltip(IMatter* m, float local_x, float local_y, float global_x, float global_y) {
    bool updated = false;
            
    return updated;
}
