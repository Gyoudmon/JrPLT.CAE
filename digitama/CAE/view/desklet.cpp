#include "desklet.hpp"

#include <plteen/datum/flonum.hpp>

using namespace Plteen;
using namespace WarGrey::CAE;

/*************************************************************************************************/
static float unit_rad = pi_f / 3.0F;

static MatterPort anchors [] = {
    MatterPort::CC, MatterPort::RC, MatterPort::RB,
    MatterPort::CB, MatterPort::LB, MatterPort::LC
};

/*************************************************************************************************/
int WarGrey::CAE::HexagonalDesklet::get_seat_by(float local_x, float local_y) {
    Dot O = this->get_bounding_box().point_at(MatterPort::CC);
    float theta = flatan(local_y - O.y, local_x - O.x);

    int idx = 0; 

    if (theta < 0.0F) {
        theta = pi_f * 2.0F + theta;
    }

    idx = int(flfloor(theta / unit_rad));

    return (idx == 0) ? 6 : idx;
}

void WarGrey::CAE::HexagonalDesklet::sit(ISprite* stu, int idx, double duration) {
    if (idx > 0) {
        auto master = this->master();
    
        if (master != nullptr) {
            float theta = (float(idx % this->seat_count()) + 0.5F) * unit_rad;
            Box box = this->get_bounding_box() * 0.5F;
            Dot O = master->get_matter_location(this, MatterPort::CC);
            float dx = 0.0F;
            float dy = 0.0F;
            
            master->glide_to(duration, stu,
                Dot(box.width() * flcos(theta), box.height() * flsin(theta)) + O,
                anchors[idx - 1], Vector::O);

            switch (idx) {
            case 1: case 4: dy = +1.0F; break;
            case 2: dx = +1.0F; dy = -1.0F; break;
            case 3: dx = +1.0F; dy = +1.0F; break;
            case 5: dx = -1.0F; dy = +1.0F; break;
            case 6: dx = -1.0F; dy = -1.0F; break;
            }

            master->glide(0.1, stu, { dx, dy });
        }
    }
}
