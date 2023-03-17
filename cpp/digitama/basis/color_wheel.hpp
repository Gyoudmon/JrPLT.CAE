#pragma once // 确保只被 include 一次

#include "../big_bang/bang.hpp"

#include <vector>

namespace WarGrey::STEM {
    class ColorWheelWorld : public WarGrey::STEM::TheBigBang {
    public:
        ColorWheelWorld() : TheBigBang("色相环", 0xFFFFFFU) {}
        virtual ~ColorWheelWorld() {}
        
    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;

    public:
        bool can_select(IMatter* m) override { return (dynamic_cast<Circlet*>(m) != nullptr) || (m == this->agent); }
        void after_select(IMatter* m, bool yes) override;
        bool update_tooltip(IMatter* m, float x, float y) override;

    private:
        std::vector<WarGrey::STEM::Circlet*> colors;
        std::vector<WarGrey::STEM::Ellipselet*> color_components;
        WarGrey::STEM::Labellet* tooltip;

    private:
        size_t selection_seq = 0; 
    };
}
