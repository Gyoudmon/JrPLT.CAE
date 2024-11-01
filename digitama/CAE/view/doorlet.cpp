#include "doorlet.hpp"

using namespace WarGrey::CAE;
using namespace Plteen;

/*************************************************************************************************/
#define DOOR_PATH digimon_path("CAE/door", "")

/*************************************************************************************************/
WarGrey::CAE::DoorSprite::DoorSprite(uint64_t seq) : Sprite(DOOR_PATH), seq(seq) {}

void WarGrey::CAE::DoorSprite::open() {
    this->play("door", 1);
}

void WarGrey::CAE::DoorSprite::close() {
    this->stop();
    this->switch_to_costume("door");
}

const char* WarGrey::CAE::DoorSprite::name() {
    static std::string s;

    s = std::to_string(this->seq);

    return s.c_str();
}
