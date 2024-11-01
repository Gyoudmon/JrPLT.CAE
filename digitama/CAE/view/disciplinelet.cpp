#include "disciplinelet.hpp"

using namespace Plteen;
using namespace WarGrey::CAE;

/*************************************************************************************************/
#define DISCIPLINE_PATH digimon_path("CAE/discipline", "")

/*************************************************************************************************/
WarGrey::CAE::DisciplineSprite::DisciplineSprite(DisciplineType type) : Sprite(DISCIPLINE_PATH), type(type) {}

int WarGrey::CAE::DisciplineSprite::get_initial_costume_index() {
    return this->costume_name_to_index(DisciplineEntity::type_to_name(this->type));
}

void WarGrey::CAE::DisciplineSprite::set_type(DisciplineType type) {
    if (this->type != type) {
        this->type = type;
        this->switch_to_costume(DisciplineEntity::type_to_name(this->type));
    }
}
