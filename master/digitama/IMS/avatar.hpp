#pragma once

#include <gydm/game.hpp>

#include <vector>

namespace WarGrey::IMS {
    /*********************************************************************************************/
    class AvatarPlane : public GYDM::Plane {
    using Plane::Plane;

    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    private:
        GYDM::Labellet* title;
        std::vector<GYDM::Labellet*> labels;
        std::vector<GYDM::TrailStudent*> avatars;
    };
}
