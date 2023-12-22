#include <gydm_stem/game.hpp>
#include <scsmos/motion/lottery.hpp>

using namespace WarGrey::STEM;
using namespace WarGrey::SCSM;

/*************************************************************************************************/
namespace {
    class LotteryMachine : public Cosmos {
    public:
        LotteryMachine() : Cosmos(60) {}

    public:
        void construct(int argc, char* argv[]) override {
            enter_digimon_zone(argv[0]);
            imgdb_setup(digimon_subdir("stone"));

#ifdef __windows__
            digimon_appdata_setup("C:\\opt\\GYDMstem\\");
            digimon_mascot_setup("C:\\opt\\GYDMstem\\stone\\mascot");
#else
            digimon_appdata_setup("/opt/GYDMstem/");
            digimon_mascot_setup("/opt/GYDMstem/stone/mascot");
#endif

            this->set_snapshot_folder("/Users/wargrey/Desktop");
            this->set_cmdwin_height(24);

            GameFont::fontsize(20);

            this->push_plane(new TwoColorLotteryPlane());
        }
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    LotteryMachine universe;

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}
