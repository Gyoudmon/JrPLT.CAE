#include "avatar.hpp"

#include <plteen/datum/fixnum.hpp>
#include <plteen/datum/flonum.hpp>

using namespace Plteen;
using namespace WarGrey::CAE;

/*************************************************************************************************/
void WarGrey::CAE::AvatarPlane::load(float width, float height) {
    auto label_font = GameFont::math();
    size_t n = TrailStudent::name_count();
    
    this->title = this->insert(new Labellet(GameFont::serif(FontSize::medium), BLACK, "%s", this->name()));

    for (size_t idx = 0; idx < n; idx ++) {
        this->labels.push_back(this->insert(new Labellet(label_font, BLACK, "%d", idx)));
        this->avatars.push_back(this->insert(new TrailStudent(idx)));
    }
}

void WarGrey::CAE::AvatarPlane::reflow(float width, float height) {
    Box tbox = this->title->get_bounding_box();
    Box grid = this->avatars[0]->get_bounding_box() + tbox.rbdot;
    float grid_width = grid.height() * 0.8F;

    for (size_t idx = 0; idx < this->labels.size(); idx ++) {
        this->move_to(this->labels[idx], { grid_width * 0.3F, grid.rbdot.y }, MatterPort::CB);
        this->move_to(this->avatars[idx], { grid_width * 1.5F, grid.rbdot.y }, MatterPort::CB);
        
        grid += Dot(0.0F, grid.height());
    }

    this->move_to(this->title, { grid_width * 1.5F, 0.0F }, MatterPort::CT);
}
