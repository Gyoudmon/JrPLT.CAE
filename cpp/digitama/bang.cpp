#include "bang.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
TTF_Font* WarGrey::STEM::bang_font::title = nullptr;
TTF_Font* WarGrey::STEM::bang_font::normal = nullptr;
TTF_Font* WarGrey::STEM::bang_font::tiny = nullptr;
TTF_Font* WarGrey::STEM::bang_font::mono = nullptr;

/*************************************************************************************************/
void WarGrey::STEM::bang_fonts_initialize() {
    if (bang_font::title == nullptr) {
        bang_font::title = game_create_font(font_basename(game_font::title), title_fontsize);
        bang_font::normal = game_create_font(font_basename(game_font::unicode), normal_fontsize);
        bang_font::tiny = game_create_font(font_basename(game_font::unicode), tiny_fontsize);
        bang_font::mono = game_create_font(font_basename(game_font::monospace), normal_fontsize);
    }
}

void WarGrey::STEM::bang_fonts_destroy() {
    game_font_destroy(bang_font::title);
    game_font_destroy(bang_font::normal);
    game_font_destroy(bang_font::tiny);
    game_font_destroy(bang_font::mono);
}

/*************************************************************************************************/
void WarGrey::STEM::TheBigBang::load(float width, float height) {
    this->title = this->insert(new Labellet(bang_font::title, BLACK, title_fmt, this->name()));

    this->agent = this->insert(new Linkmon());
    this->agent->scale(-1.0F, 1.0F);

    this->set_sentry_sprite(this->agent, "Greeting", "GoodBye");
    this->move_to(this->title, this->agent, MatterAnchor::RB, MatterAnchor::LB);
}
