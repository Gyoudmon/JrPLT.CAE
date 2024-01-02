#pragma once

#include <gydm/game.hpp>

namespace WarGrey::IMS {
    class IDesk {
    public:
        IDesk(size_t idx) : idx(idx) {}
        virtual ~IDesk() {}

    public:
        virtual size_t seat_count() = 0;
        virtual int get_seat_by(float local_x, float local_y) = 0;
        virtual void sit(GYDM::ISprite* stu, int idx, double duration = 0.0) = 0;

    public:
        size_t get_index() { return this->idx; }

    private:
        size_t idx;
    };

    class HexagonalDesklet : public GYDM::RegularPolygonlet, public WarGrey::IMS::IDesk {
    public:
        HexagonalDesklet(size_t idx, float radius, const GYDM::RGBA& color, const GYDM::RGBA& border_color = GYDM::transparent)
            : GYDM::RegularPolygonlet(6, radius, color, border_color), IDesk(idx) {}
        virtual ~HexagonalDesklet() noexcept {}

    public:
        size_t seat_count() override { return 6; }
        int get_seat_by(float local_x, float local_y) override;
        void sit(GYDM::ISprite* stu, int idx, double duration = 0.0) override;
    };
}
