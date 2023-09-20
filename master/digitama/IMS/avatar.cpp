#include "avatar.hpp"

#include <gydm_stem/datum/fixnum.hpp>
#include <gydm_stem/datum/flonum.hpp>

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
void WarGrey::IMS::AvatarPlane::load(float width, float height) {
    auto label_font = GameFont::math();
    size_t n = TrailStudent::name_count();
    
    this->title = this->insert(new Labellet(GameFont::serif(FontSize::medium), BLACK, "%s", this->name()));

    for (size_t idx = 0; idx < n; idx ++) {
        this->labels.push_back(this->insert(new Labellet(label_font, BLACK, "%d", idx)));
        this->avatars.push_back(this->insert(new TrailStudent(idx)));
    }
}

void WarGrey::IMS::AvatarPlane::reflow(float width, float height) {
    float grid_height = 0.0F;
    float grid_y, grid_width;

    this->title->feed_extent(0.0F, 0.0F, nullptr, &grid_y);
    this->avatars[0]->feed_extent(0.0F, 0.0F, nullptr, &grid_height);

    grid_width = grid_height * 0.8F;
    for (size_t idx = 0; idx < this->labels.size(); idx ++) {
        this->move_to(this->labels[idx], grid_width * 0.3F, grid_y + grid_height, MatterAnchor::CB);
        this->move_to(this->avatars[idx], grid_width * 1.5F, grid_y + grid_height, MatterAnchor::CB);
        
        grid_y += grid_height;
    }

    this->move_to(this->title, grid_width * 1.5F, 0.0F, MatterAnchor::CT);
}
