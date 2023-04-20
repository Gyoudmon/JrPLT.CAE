#pragma once // 确保只被 include 一次

#include "../big_bang/bang.hpp"

#include "dewdney/steppe.hpp"
#include "dewdney/animal.hpp"

#include <vector>

namespace WarGrey::STEM {
    /*********************************************************************************************/
    class EvolutionWorld : public WarGrey::STEM::TheBigBang {
    public:
        EvolutionWorld(float size_hint = 32.0F) : EvolutionWorld("", size_hint) {}
        EvolutionWorld(const std::string& world, float size_hint = 32.0F)
            : TheBigBang("演化游戏"), world_path(world), size_hint(size_hint) {}
        virtual ~EvolutionWorld() {}
        
    public:
        void load(float width, float height) override;
        void reflow(float width, float height) override;
        void update(uint64_t count, uint32_t interval, uint64_t uptime) override;
        void on_mission_start(float width, float height) override;

    public:
        bool can_select(WarGrey::STEM::IMatter* m) override;

    protected:
        void after_select(WarGrey::STEM::IMatter* m, bool yes) override;
        bool update_tooltip(WarGrey::STEM::IMatter* m, float lx, float ly, float gx, float gy) override;

    private:
        void animal_live(Animal* animal, IToroidalMovingAnimal* self, float tile_width, float tile_height);
        void clear_dead_animals();

    private:
        void reset_world();
        void update_dayinfo();
            
    private: /* 本世界中的物体 */
        WarGrey::STEM::SteppeAtlas* steppe;
        std::vector<WarGrey::STEM::Animal*> animals;
        WarGrey::STEM::Labellet* day_info;
 
    private: /* 本世界的参数设定 */
        int row;
        int col;

    private:
        std::string world_path;
        float size_hint;
    };
}
