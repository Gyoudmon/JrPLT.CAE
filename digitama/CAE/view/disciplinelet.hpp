#pragma once

#include <plteen/game.hpp>

#include "../entity/discipline.hpp"

namespace WarGrey::CAE {
    class DisciplineSprite : public Plteen::Sprite {
    public:
        DisciplineSprite(DisciplineType type);
        virtual ~DisciplineSprite() {}

    public:
        const char* name() override { return DisciplineEntity::type_to_name(this->type); }

    public:
        void set_type(DisciplineType type);
        DisciplineType get_type() { return this->type; }

    protected:
        virtual int get_initial_costume_index() override;

    private:
        DisciplineType type;
    };
}
