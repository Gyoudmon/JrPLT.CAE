#include "color_wheel.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const size_t color_count = 36U;
static const float color_radius = 16.0F;
static const float wheel_radius = 360.0F;

static const float color_mixture_radius = 100.0F;

/*************************************************************************************************/
void WarGrey::STEM::ColorWheelWorld::load(float width, float height) {
    this->set_background(0x000000U);

    this->color_components.push_back(this->insert(new Ellipselet(color_mixture_radius, 0xFF0000U)));
    this->color_components.push_back(this->insert(new Ellipselet(color_mixture_radius, 0x00FF00U)));
    this->color_components.push_back(this->insert(new Ellipselet(color_mixture_radius, 0x0000FFU)));

    for (auto com : this->color_components) {
        com->set_color_mixture(ColorMixture::Add);
    }

    this->load_color_wheel_components();
    TheBigBang::load(width, height);

    this->tooltip = this->insert(make_label_for_tooltip(GameFont::Tooltip()));
    this->set_tooltip_matter(this->tooltip);
}

void WarGrey::STEM::ColorWheelWorld::reflow(float width, float height) {
    float delta_deg = 360.0F / float(color_count);
    float cx = width * 0.5F;
    float cy = height * 0.55F;
    float x, y;
    int idx = 0;

    TheBigBang::reflow(width, height);
    
    for (float deg = 0.0F; deg < 360.0F; deg += delta_deg) {
        circle_point(wheel_radius, deg - 90.0F, &x, &y, false);
        this->move_to(this->colors[idx++], cx + x, cy + y, MatterAnchor::CC);
    }

    this->reflow_color_components(width * 0.5F, height * 0.5F);
}

void WarGrey::STEM::ColorWheelWorld::after_select(IMatter* m, bool yes) {
    if (yes) {
        auto com = dynamic_cast<Circlet*>(m);

        if (com != nullptr) {
            this->color_components[this->selection_seq]->set_color(com->get_color());
            this->selection_seq = (this->selection_seq + 1) % this->color_components.size();
        }
    }
}

bool WarGrey::STEM::ColorWheelWorld::update_tooltip(IMatter* m, float x, float y, float gx, float gy) {
    bool updated = false;
    auto com = dynamic_cast<Circlet*>(m);
    auto cc = dynamic_cast<Ellipselet*>(m);

    if (com != nullptr) {
        this->tooltip->set_text(" #%06X [Hue: %.2f] ", com->get_color(), com->get_body_hsb_hue());
        this->no_selected();
        updated = true;
    } else if (cc != nullptr) {
        uint32_t hex = 0U;

        for (size_t idx = 0; idx < this->color_components.size(); idx ++) {
            float cx, cy;
            
            this->feed_matter_location(this->color_components[idx], &cx, &cy, MatterAnchor::CC);

            if (point_distance(gx, gy, cx, cy) <= color_mixture_radius) {
                hex = RGB_Add(hex, static_cast<uint32_t>(this->color_components[idx]->get_color()));
            }
        }

        this->tooltip->set_text(" #%06X ", hex);
        updated = true;
    }

    return updated;
}

/*************************************************************************************************/
void WarGrey::STEM::ColorWheelWorld::load_color_wheel_components() {
    float delta_deg = 360.0F / float(color_count);

    for (float deg = 0.0F; deg < 360.0F; deg += delta_deg) {
        this->colors.push_back(this->insert(new Circlet(color_radius, deg)));
    }
}

void WarGrey::STEM::ColorWheelWorld::reflow_color_components(float x, float y) {
    float cc_off = color_mixture_radius * 0.5F;
    
    this->move_to(this->color_components[0], x, y, MatterAnchor::CB, 0.0F, cc_off);
    this->move_to(this->color_components[1], this->color_components[0], MatterAnchor::CB, MatterAnchor::RC, cc_off);
    this->move_to(this->color_components[2], this->color_components[1], MatterAnchor::CC, MatterAnchor::LC);
}
