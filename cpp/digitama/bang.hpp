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
            TheBigBang(const char* name = unknown_task_name)
                : WarGrey::STEM::Plane(name) {}

            virtual ~TheBigBang() {}

        public:
            void load(float width, float height) override;
            void reflow(float width, float height) override;

        public:
            bool can_select(WarGrey::STEM::IMatter* m) override;
            void after_select(WarGrey::STEM::IMatter* m, bool yes_or_no) override;
            bool has_mission_completed() override;
            void on_enter(WarGrey::STEM::IPlane* from) override;
            void on_leave(WarGrey::STEM::IPlane* to) override;

        protected:
            virtual void on_enter_task() {}
            virtual void on_leave_task() {}

        protected:
            WarGrey::STEM::AgentSpriteSheet* agent;
            WarGrey::STEM::Labellet* title;

        private:
            bool task_done = false;
    };
}
