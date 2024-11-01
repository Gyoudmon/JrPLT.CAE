#include "studentlet.hpp"

using namespace Plteen;
using namespace WarGrey::CAE;

/*************************************************************************************************/
WarGrey::CAE::StudentSprite::StudentSprite(uint64_t No, std::string nickname, uint8_t avatar)
    : Citizen(avatar, TRAIL_STUDENTS_PATH), No(No), nickname(nickname), name_texture(nullptr) {}

void WarGrey::CAE::StudentSprite::draw(dc_t* dc, float x, float y, float Width, float Height) {
    Sprite::draw(dc, x, y, Width, Height);

    if (this->sbar_color.is_opacity() && (this->sbar_percentage > 0.0)) {
        dc->draw_hline(x, y + 1.0F, Width * this->sbar_percentage, this->sbar_color);
    }

    if (this->name_texture.use_count() == 0) {
        this->name_texture = std::make_shared<Texture>(dc->create_blended_text(this->nickname,
                                                        GameFont::fangsong(FontSize::xx_small), SNOW));
        this->name_texture->feed_extent(&this->name_region.w, &this->name_region.h);
    }

    if (this->name_texture.use_count() > 0) {
        this->name_region.x = x + (Width - this->name_region.w) * 0.5F;
        this->name_region.y = y + Height - this->name_region.h;

        dc->fill_rect(x, this->name_region.y, Width, this->name_region.h, RGBA(BLACK, 0.48));
        dc->stamp(this->name_texture->self(), this->name_region.x, this->name_region.y);
    }
}

void WarGrey::CAE::StudentSprite::set_nickname(const std::string& name) {
    if (this->nickname.compare(name) != 0) {
        this->nickname = name;
        this->name_texture.reset();
        this->notify_updated();
    }
}

void WarGrey::CAE::StudentSprite::set_score_percentage(double percentage, const RGBA& color) {
    if ((this->sbar_percentage != percentage) || (color != this->sbar_color)) {
        this->sbar_percentage = percentage;
        this->sbar_color = color;
        
        this->notify_updated();
    }
}
