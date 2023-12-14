#pragma once

#include <gydm_stem/game.hpp>

namespace WarGrey::IMS {
    class IDesk {
    public:
        IDesk(size_t idx) : idx(idx) {}
        virtual ~IDesk() {}

    public:
        virtual size_t seat_count() = 0;
        virtual int get_seat_by(float local_x, float local_y) = 0;
        virtual void sit(WarGrey::STEM::ISprite* stu, int idx, double duration = 0.0) = 0;

    public:
        size_t get_index() { return this->idx; }

    private:
        size_t idx;
    };

    class HexagonalDesklet : public WarGrey::STEM::RegularPolygonlet, public WarGrey::IMS::IDesk {
    public:
        HexagonalDesklet(size_t idx, float radius, const WarGrey::STEM::RGBA& color, const WarGrey::STEM::RGBA& border_color = WarGrey::STEM::transparent)
            : WarGrey::STEM::RegularPolygonlet(6, radius, color, border_color), IDesk(idx) {}
        virtual ~HexagonalDesklet() noexcept {}

    public:
        size_t seat_count() override { return 6; }
        int get_seat_by(float local_x, float local_y) override;
        void sit(WarGrey::STEM::ISprite* stu, int idx, double duration = 0.0) override;
    };
}
