#include "discipline.hpp"

#include <strings.h>

using namespace WarGrey::CAE;
using namespace Plteen;

/*************************************************************************************************/
bool WarGrey::CAE::DisciplineEntity::match(const std::string& line, int* offset) {
    return GMSEntity::match(line, discipline_mark, offset);
}

const char* WarGrey::CAE::DisciplineEntity::prompt() {
    return "{ code:nat, name:str }";
}

const char* WarGrey::CAE::DisciplineEntity::type_to_name(DisciplineType type) {
    switch (type) {
    case DisciplineType::Mathematics: return "Mathematics";
    case DisciplineType::Programming: return "Programming";
    case DisciplineType::STEM: return "STEM";
    case DisciplineType::Printing3D: return "3DPrinting";
    case DisciplineType::Physics: return "Physics";
    case DisciplineType::Chemistry: return "Chemistry";
    case DisciplineType::Biology: return "Biology";
    default: return "Blank";
    }
}

DisciplineType WarGrey::CAE::DisciplineEntity::name_to_type(const char* name) {
    if ((strcasecmp(name, "mathematics") == 0) || (strcasecmp(name, "math") == 0)) {
        return DisciplineType::Mathematics;
    } else if ((strcasecmp(name, "programming") == 0) || (strcasecmp(name, "coding") == 0)) {
        return DisciplineType::Programming;
    } else if (strcasecmp(name, "STEM") == 0) {
        return DisciplineType::STEM;
    } else if ((strcasecmp(name, "3dprinting") == 0) || (strcasecmp(name, "3d") == 0)) {
        return DisciplineType::Printing3D;
    } else if (strcasecmp(name, "physics") == 0) {
        return DisciplineType::Physics;
    } else if (strcasecmp(name, "chemistry") == 0) {
        return DisciplineType::Chemistry;
    } else if ((strcasecmp(name, "biology") == 0) || (strcasecmp(name, "bio") == 0)) {
        return DisciplineType::Biology;
    } else {
        return DisciplineType::Blank;
    }
}

/*************************************************************************************************/
WarGrey::CAE::DisciplineEntity::DisciplineEntity(const std::string& s, int idx) {
    size_t pos = size_t(idx);
    size_t end = s.size();
    const char* src = s.c_str();

    scan_skip_space(src, &pos, end);

    this->code = scan_natural(src, &pos, end);
    if (this->code == 0U) throw exn_gms("无效课程代号");
    scan_skip_delimiter(src, &pos, end, field_delimiter);

    this->type = name_to_type(scan_string(src, &pos, end, field_delimiter).c_str());
}

std::string WarGrey::CAE::DisciplineEntity::to_string() {
    return make_nstring("%c:%llu,%s", discipline_mark, this->code, this->cannonical_name());
}

const char* WarGrey::CAE::DisciplineEntity::cannonical_name() {
    return DisciplineEntity::type_to_name(this->type);
}
