#pragma once

#include <plteen/game.hpp>

#include <vector>

namespace WarGrey::CAE {
    /*********************************************************************************************/
    class AvatarPlane : public Plteen::Plane {
    using Plane::Plane;

    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    private:
        Plteen::Labellet* title;
        std::vector<Plteen::Labellet*> labels;
        std::vector<Plteen::TrailStudent*> avatars;
    };
}
