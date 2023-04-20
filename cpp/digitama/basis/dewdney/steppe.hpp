#pragma once // 确保只被 include 一次

#include "../../big_bang/bang.hpp"

namespace WarGrey::STEM {
    static const GroundBlockType fossil_tile_type = GroundBlockType::Dirt;
    static const GroundBlockType steppe_tile_type = GroundBlockType::Plain;
    static const GroundBlockType plant_tile_type = GroundBlockType::Grass;

    /*********************************************************************************************/
    class SteppeAtlas : public WarGrey::STEM::PlanetCuteAtlas {
    public:
        SteppeAtlas(int row, int col);
        virtual ~SteppeAtlas() {}

    public:
        int preferred_local_fps() override { return 4; }
        int update(uint64_t count, uint32_t interval, uint64_t uptime) override;

    public:
        void reset();
        int current_day() { return this->day; }

    private:
        void random_plant(int r0, int c0, int row_size, int col_size);

    private:
        int day;
        int jungle_r;
        int jungle_c;
        int jungle_row;
        int jungle_col;
    };
}
