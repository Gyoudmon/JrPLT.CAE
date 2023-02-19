#include "digitama/splash.hpp"

using namespace WarGrey::STEM;

namespace {
    enum class CmdlineOps { _ };

    class BigBangCosmos : public TheCosmos {
    public:
        BigBangCosmos(int fps = 60) : TheCosmos(fps) {}
        virtual ~BigBangCosmos() {}

    public:
        void construct(int argc, char* argv[]) override {
            this->set_window_size(1200, 0);

            TheCosmos::construct(argc, argv);
        }

    protected:
        void parse_cmdline_options(int argc, char* argv[]) override {}
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    /* 混沌初开，宇宙诞生 */
    BigBangCosmos universe;

    /* 创造游戏世界 */
    universe.construct(argc, args);

    /**
     * 宇宙大爆炸
     * 开启游戏主循环，直到玩家关闭游戏
     **/
    universe.big_bang();

    /**
     * C++ 心满意足地退出
     * 顺便销毁游戏宇宙，回归虚无
     **/
    return 0;
}
