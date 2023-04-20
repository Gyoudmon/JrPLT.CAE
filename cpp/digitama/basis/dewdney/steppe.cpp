#include "steppe.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
WarGrey::STEM::SteppeAtlas::SteppeAtlas(int row, int col) : PlanetCuteAtlas(row, col, steppe_tile_type) {
    this->jungle_row = 6 + row % 2;
    this->jungle_col = 4 + col % 2;

    this->jungle_r = (row - this->jungle_row) / 2;
    this->jungle_c = (col - this->jungle_col) / 2;
}

int WarGrey::STEM::SteppeAtlas::update(uint64_t count, uint32_t interval, uint64_t uptime) {
    this->random_plant(this->jungle_r, this->jungle_c, this->jungle_row, this->jungle_col);
    this->random_plant(0, 0, this->map_row, this->map_col);
    
    return 0;
}

void WarGrey::STEM::SteppeAtlas::random_plant(int r0, int c0, int row_size, int col_size) {
    int r = random_uniform(0, row_size - 1) + r0;
    int c = random_uniform(0, col_size - 1) + c0;

    this->set_tile_type(r, c, plant_tile_type);
}

void WarGrey::STEM::SteppeAtlas::reset() {
    for (int r = 0; r < this->map_row; r ++) {
        for (int c = 0; c < this->map_col; c ++) {
            this->set_tile_type(r, c, steppe_tile_type);
        }
    }

    this->day = 0;
}
