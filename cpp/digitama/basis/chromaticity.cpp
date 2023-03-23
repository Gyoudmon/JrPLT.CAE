#include "chromaticity.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const size_t color_count = 36U;
static const float color_radius = 16.0F;
static const float wheel_radius = 360.0F;

static const float color_mixture_radius = 64.0F;
static const float chromaticity_size = 380.0F;

/*************************************************************************************************/
void WarGrey::STEM::ChromaticityDiagramWorld::load(float width, float height) {
    float delta_deg = 360.0F / float(color_count);

    this->set_background(0x000000U);

    for (float deg = 0.0F; deg < 360.0F; deg += delta_deg) {
        this->colors.push_back(this->insert(new Circlet(color_radius, deg)));
    }

    this->color_components.push_back(this->insert(new Ellipselet(color_mixture_radius, 0xFF0000U)));
    this->color_components.push_back(this->insert(new Ellipselet(color_mixture_radius, 0x00FF00U)));
    this->color_components.push_back(this->insert(new Ellipselet(color_mixture_radius, 0x0000FFU)));
    this->chroma_dia = this->insert(new Chromalet(chromaticity_size));

    for (auto com : this->color_components) {
        com->set_color_mixture(ColorMixture::Add);
    }

    TheBigBang::load(width, height);

    this->tooltip = this->insert(make_label_for_tooltip(GameFont::Tooltip()));
    this->set_tooltip_matter(this->tooltip);
}

void WarGrey::STEM::ChromaticityDiagramWorld::reflow(float width, float height) {
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

    circle_point(wheel_radius, -90.0F, &x, &y, false);
    this->reflow_color_components(cx + x, cy + y + (color_mixture_radius + color_radius) * 1.618F);
    this->move_to(this->chroma_dia, width * 0.5F, height * 0.618F, MatterAnchor::CC);
}

void WarGrey::STEM::ChromaticityDiagramWorld::update(uint32_t interval, uint32_t count, uint32_t uptime) {
    if (is_shift_pressed()) {
        this->chroma_dia->set_pseudo_primary_triangle_alpha(0.32);
    } else {
        this->chroma_dia->set_pseudo_primary_triangle_alpha(0.00);
    }
}

bool WarGrey::STEM::ChromaticityDiagramWorld::can_select(IMatter* m) {
    return (dynamic_cast<Circlet*>(m) != nullptr)
            || (m == this->agent)
            || (m == this->chroma_dia);
}

void WarGrey::STEM::ChromaticityDiagramWorld::after_select(IMatter* m, bool yes) {
    if (yes) {
        auto com = dynamic_cast<Circlet*>(m);

        if (com != nullptr) {
            uint32_t pcolor = static_cast<uint32_t>(com->get_color());

            this->color_components[this->selection_seq]->set_color(pcolor);
            this->chroma_dia->set_pseudo_primary_color(pcolor, this->selection_seq);
            this->selection_seq = (this->selection_seq + 1) % this->color_components.size();
        } else if (m == this->chroma_dia) {
            switch (this->chroma_dia->get_standard()) {
            case CIE_Standard::Primary: this->chroma_dia->set_standard(CIE_Standard::D65); break;
            case CIE_Standard::D65: this->chroma_dia->set_standard(CIE_Standard::Primary); break;
            }

            this->no_selected();
        }
    }
}

bool WarGrey::STEM::ChromaticityDiagramWorld::update_tooltip(IMatter* m, float x, float y, float gx, float gy) {
    bool updated = false;
    auto com = dynamic_cast<Circlet*>(m);
    auto cc = dynamic_cast<Ellipselet*>(m);

    if (com != nullptr) {
        uint32_t hex = com->get_color();

        this->tooltip->set_text(" #%06X [Hue: %.2f] ", hex, com->get_body_hsb_hue());
        this->tooltip->set_background_color(GHOSTWHITE);

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
        this->tooltip->set_background_color(GHOSTWHITE);
        updated = true;
    } else if (m == this->chroma_dia) {
        uint32_t hex = this->chroma_dia->get_color_at(x, y, is_shift_pressed());

        if (hex > 0U) {
            double co_x, co_y;

            this->chroma_dia->feed_color_location(hex, nullptr, nullptr, &co_x, &co_y);
            
            switch (this->chroma_dia->get_standard()) {
                case CIE_Standard::Primary: {
                    this->tooltip->set_text(" CIE Primary：%06X (%.3lf, %.3lf, %.3lf) ",
                        hex, co_x, co_y, 1.0 - co_x - co_y);
                 }; break;
                case CIE_Standard::D65: {
                    this->tooltip->set_text(" CIE sRGB-D65: %06X (%.3lf, %.3lf, %.3lf) ",
                        hex, co_x, co_y, 1.0 - co_x - co_y);
                 }; break;
            }

            this->tooltip->set_background_color(hex);
            updated = true;
        }
    }

    return updated;
}

void WarGrey::STEM::ChromaticityDiagramWorld::reflow_color_components(float x, float y) {
    float cc_off = color_mixture_radius * 0.5F;
    
    this->move_to(this->color_components[0], x, y, MatterAnchor::CB, 0.0F, cc_off);
    this->move_to(this->color_components[1], this->color_components[0], MatterAnchor::CB, MatterAnchor::RC, cc_off);
    this->move_to(this->color_components[2], this->color_components[1], MatterAnchor::CC, MatterAnchor::LC);
}
