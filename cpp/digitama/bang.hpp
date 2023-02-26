#pragma once

#include "big_bang/game.hpp"
#include "big_bang/physics/random.hpp"

namespace WarGrey::STEM {
    /* Constants */
    static const char* title_fmt = "青少计算机科学: %s";
    static const char* unknown_task_name = "冒险越来越深入了";
    
    /* Fonts */
    static const int title_fontsize = 42; /* <-- already hit the ceiling */
    static const int normal_fontsize = 24;
    static const int tiny_fontsize = 20;

    class bang_font {
    public:
        static TTF_Font* title;
        static TTF_Font* normal;
        static TTF_Font* tiny;
        static TTF_Font* mono;
    };

    void bang_fonts_initialize();
    void bang_fonts_destroy();

    /*********************************************************************************************/
    class TheBigBang : public WarGrey::STEM::Plane {
        public:
            TheBigBang(const char* name = unknown_task_name, uint32_t title_color = BLACK)
                : WarGrey::STEM::Plane(name), title_color(title_color) {}
            virtual ~TheBigBang() {}

        public:
            void load(float width, float height) override;

        protected:
            WarGrey::STEM::AgentSpriteSheet* agent;
            WarGrey::STEM::Labellet* title;

        private:
            uint32_t title_color;
    };
}
