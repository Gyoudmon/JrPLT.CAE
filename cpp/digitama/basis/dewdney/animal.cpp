#include "animal.hpp"

#include "../../big_bang/datum/fixnum.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
static const float lifebar_height = 2.0F;
static const double lifebar_alpha = 0.16;

static const int orientate_gene[MOVING_WAYS] = { 1, 1, 10, 1, 1, 1, 1, 1 };
static const int random_gene[MOVING_WAYS] = { 1, 1, 1, 1, 1, 1, 1, 1 };

/*************************************************************************************************/
WarGrey::STEM::IToroidalMovingAnimal::IToroidalMovingAnimal(int row, int col, const int gene[MOVING_WAYS], double duration, int direction, int energy)
        : duration(duration), row(row), col(col), direction(direction), energy(energy) {
    this->r = row / 2;
    this->c = col / 2;

    this->full_energy = energy;
    this->reproduce_energy = energy / 5;

    for (int idx = 0; idx < MOVING_WAYS; idx ++) {
        this->gene[idx] = gene[idx];
    }
}

void WarGrey::STEM::IToroidalMovingAnimal::draw(SDL_Renderer* render, float x, float y, float width, float height) {
    float lifebar_width = float(this->energy) / float(this->full_energy);

    if (this->energy >= this->reproduce_energy) {
        game_draw_rect(render, x, y, lifebar_width * width, lifebar_height, ROYALBLUE, lifebar_alpha);
    } else {
        game_draw_rect(render, x, y, lifebar_width * width, lifebar_height, CRIMSON, lifebar_alpha);
    }
}

void WarGrey::STEM::IToroidalMovingAnimal::turn() {
    int sum = 0, rnd;

    for (int idx = 0; idx < MOVING_WAYS; idx ++) {
        sum += this->gene[idx];
    }

    rnd = random_uniform(0, sum - 1);
    this->direction = safe_index(this->direction + this->angle(0, rnd), MOVING_WAYS);
}

int WarGrey::STEM::IToroidalMovingAnimal::angle(int idx0, int rnd) {
    int next = rnd - this->gene[idx0];

    if (next < 0) {
        return 0;
    } else {
        return this->angle(idx0 + 1, next) + 1;
    }
}

void WarGrey::STEM::IToroidalMovingAnimal::move(int* delta_row, int* delta_col) {
    int dr = 0;
    int dc = 0;
    int orow = this->r;
    int ocol = this->c;

    switch (this->direction) {
    case 0: dr = dc = -1; break;
    case 1: dr = -1; break;
    case 2: dr = -1; dc = +1; break;
    case 3: dc = +1; break;
    case 4: dc = dr = +1; break;
    case 5: dr = +1; break;
    case 6: dr = +1; dc = -1; break;
    case 7: dc = -1; break;
    default: /* deadcode */;
    }

    this->r = safe_index(orow + dr, this->row);
    this->c = safe_index(ocol + dc, this->col);
    this->energy -= 1;

    SET_BOX(delta_row, this->r - orow);
    SET_BOX(delta_col, this->c - ocol);
}

/*************************************************************************************************/
WarGrey::STEM::TMRooster::TMRooster(int row, int col, int direction, int energy) {
    this->attach_metadata(new IToroidalMovingAnimal(row, col, orientate_gene, 0.5, direction, energy));
}

void WarGrey::STEM::TMRooster::draw(SDL_Renderer* render, float x, float y, float width, float height) {
    Rooster::draw(render, x, y, width, height);
    this->unsafe_metadata<IToroidalMovingAnimal>()->draw(render, x, y, width, height);
}

WarGrey::STEM::TMCow::TMCow(int row, int col, int direction, int energy) {
    this->attach_metadata(new IToroidalMovingAnimal(row, col, random_gene, 2.0, direction, energy));
}

void WarGrey::STEM::TMCow::draw(SDL_Renderer* render, float x, float y, float width, float height) {
    Cow::draw(render, x, y, width, height);
    this->unsafe_metadata<IToroidalMovingAnimal>()->draw(render, x, y, width, height);
}

WarGrey::STEM::TMCat::TMCat(int row, int col, int direction, int energy) {
    this->attach_metadata(new IToroidalMovingAnimal(row, col, random_gene, 0.4, direction, energy));
}

void WarGrey::STEM::TMCat::draw(SDL_Renderer* render, float x, float y, float width, float height) {
    Cat::draw(render, x, y, width, height);
    this->unsafe_metadata<IToroidalMovingAnimal>()->draw(render, x, y, width, height);
}
