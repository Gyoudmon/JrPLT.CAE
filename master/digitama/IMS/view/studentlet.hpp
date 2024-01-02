#pragma once

#include <gydm/game.hpp>

#include "../entity/student.hpp"

namespace WarGrey::IMS {
    class StudentSprite : public GYDM::Citizen {
    public:
        StudentSprite(uint64_t No, std::string nickname, uint8_t avatar);
        virtual ~StudentSprite() {}

    public:
        void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override;

    public:
        const char* name() override { return this->nickname.c_str(); }
        uint64_t primary_key() { return this->No; }

    public:
        void set_nickname(const std::string& name);
        void set_score_percentage(double percentage, const GYDM::RGBA& color = 0x00FF00U);

    private:
        uint64_t No;
        std::string nickname;

    private:
        GYDM::RGBA sbar_color = GYDM::transparent;
        double sbar_percentage = 0.0;

    private:
        GYDM::shared_texture_t name_texture;
        SDL_FRect name_region;
    };
}
