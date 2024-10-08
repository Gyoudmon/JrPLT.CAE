#include <plteen/game.hpp>
#include <stemos/motion/lottery.hpp>

using namespace Plteen;
using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    class LotteryMachineWithCodePlane : public LotteryPlane {
    public:
        LotteryMachineWithCodePlane() : LotteryPlane() {}

    public:
        void load(float width, float height) override {
            this->ball_number_cpp = this->load_code("Lottery/numbers", 1.0F);
            this->initialization_cpp = this->load_code("Lottery/initialization", 1.0F);
            this->applying_forces_cpp = this->load_code("Lottery/forces", 1.0F);

            LotteryPlane::load(width, height);
        }

        void reflow(float width, float height) override {
            LotteryPlane::reflow(width, height);

            this->move_to(this->ball_number_cpp, { width, height * 0.5F }, MatterPort::RC);
            this->move_to(this->initialization_cpp, { 0.0F, height * 0.5F }, MatterPort::LB);
            this->move_to(this->applying_forces_cpp, { 0.0F, height * 0.6F }, MatterPort::LT);
        }

    private:
        Sprite* load_code(const char* path, float scale = 1.0F) {
            Sprite* code = this->insert(new Sprite(digimon_path(path, ".png")));

            if (scale != 1.0F) {
                code->scale(scale);
            }

            return code;
        }

    private:
        Sprite* ball_number_cpp;
        Sprite* initialization_cpp;
        Sprite* applying_forces_cpp;
    };

    class LotteryMachine : public Cosmos {
    public:
        LotteryMachine() : Cosmos(60) {}

    public:
        void construct(int argc, char* argv[]) override {
            GameFont::fontsize(20);
            
            enter_digimon_zone(argv[0]);
            imgdb_setup(digimon_subdir("stone"));
            this->set_snapshot_folder("/Users/wargrey/Desktop");
            this->set_window_fullscreen(true);

#ifdef __windows__
            digimon_appdata_setup("C:\\opt\\Plteenstem\\");
            digimon_mascot_setup("C:\\opt\\Plteenstem\\stone\\mascot");
#else
            digimon_appdata_setup("/opt/Plteenstem/");
            digimon_mascot_setup("/opt/Plteenstem/stone/mascot");
#endif

            this->push_plane(new LotteryMachineWithCodePlane());
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
