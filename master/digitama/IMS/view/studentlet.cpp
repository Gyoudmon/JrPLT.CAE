#include "studentlet.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::IMS;

/*************************************************************************************************/
WarGrey::IMS::StudentSprite::StudentSprite(uint64_t No, std::string nickname, uint8_t avatar)
    : Citizen(avatar, TRAIL_STUDENTS_PATH), No(No), nickname(nickname), name_texture(nullptr) {}

void WarGrey::IMS::StudentSprite::draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) {
    Sprite::draw(renderer, x, y, Width, Height);

    if ((this->sbar_alpha > 0.0) && (this->sbar_percentage > 0.0)) {
        float ly = y + 1.0F;

        game_draw_line(renderer, x, ly,
            x + Width * this->sbar_percentage, ly,
            this->sbar_color, this->sbar_alpha);
    }

    if (this->name_texture.use_count() == 0) {
        this->name_texture = std::make_shared<Texture>(game_blended_text_texture(renderer, this->nickname,
            GameFont::fangsong(FontSize::xx_small), SNOW));
        this->name_texture->feed_extent(&this->name_region.w, &this->name_region.h);
    }

    if (this->name_texture.use_count() > 0) {
        this->name_region.x = x + (Width - this->name_region.w) * 0.5F;
        this->name_region.y = y + Height - this->name_region.h;

        game_fill_rect(renderer, x, this->name_region.y, Width, this->name_region.h, BLACK, 0.48);
        game_draw_image(renderer, this->name_texture->self(), this->name_region.x, this->name_region.y);
    }
}

void WarGrey::IMS::StudentSprite::set_nickname(const std::string& name) {
    if (this->nickname.compare(name) != 0) {
        this->nickname = name;
        this->name_texture.reset();
        this->notify_updated();
    }
}

void WarGrey::IMS::StudentSprite::set_score_percentage(double percentage, uint32_t color, double alpha) {
    if ((this->sbar_percentage != percentage) || (color != this->sbar_color) || (alpha != this->sbar_alpha)) {
        this->sbar_percentage = percentage;
        this->sbar_color = color;
        this->sbar_alpha = alpha;

        this->notify_updated();
    }
}
