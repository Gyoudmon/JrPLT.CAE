#pragma once // 确保只被 include 一次

#include "../../big_bang/bang.hpp"

#include <vector>

namespace WarGrey::STEM {
    static const int MOVING_WAYS = 8;

    /*********************************************************************************************/
    class IToroidalMovingAnimal : public WarGrey::STEM::IMatterMetadata {
    public:
        IToroidalMovingAnimal(int row, int col, const int gene[MOVING_WAYS], double duration, int direction, int energy);
        virtual ~IToroidalMovingAnimal() {}

    public:
        void draw(SDL_Renderer* renderer, float x, float y, float width, float height);

    public:
        void turn();
        void move(int* dr = nullptr, int* dc = nullptr);
        bool is_alive() const { return this->energy > 0; }

    public:
        double pace_duration() { return this->duration; }
        int current_row() { return r; }
        int current_col() { return c; }

    private:
        int angle(int idx0, int rnd);

    private:
        double duration;
        int full_energy;
        int reproduce_energy;
        int gene[MOVING_WAYS];
        int row;
        int col;

    private:
        int direction;
        int energy;
        int r;
        int c;
    };

    /*********************************************************************************************/
    class TMRooster : public WarGrey::STEM::Rooster {
    public:
        TMRooster(int row, int col, int direction = 0, int energy = 300);
        virtual ~TMRooster() {}

    public:
        void draw(SDL_Renderer* renderer, float x, float y, float width, float height) override;
    };

    class TMCow : public WarGrey::STEM::Cow {
    public:
        TMCow(int row, int col, int direction = 0, int energy = 600);
        virtual ~TMCow() {}

    public:
        void draw(SDL_Renderer* renderer, float x, float y, float width, float height) override;
    };

    class TMCat : public WarGrey::STEM::Cat {
    public:
        TMCat(int row, int col, int direction = 0, int energy = 1000);
        virtual ~TMCat() {}

    public:
        void draw(SDL_Renderer* renderer, float x, float y, float width, float height) override;
    };
}
