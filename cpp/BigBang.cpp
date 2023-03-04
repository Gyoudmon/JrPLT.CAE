#include "digitama/splash.hpp"
#include "digitama/gallery.hpp"

#include "digitama/pltmos/stream.hpp"

#include "shape.hpp"
#include "paddleball.hpp"

using namespace WarGrey::STEM;
using namespace WarGrey::PLT;

/*************************************************************************************************/
namespace {
    enum class CmdlineOps { StreamFile, _ };

    class BigBangCosmos : public TheCosmos {
    public:
        BigBangCosmos(int fps = 60) : TheCosmos(fps) {}
        virtual ~BigBangCosmos() {}

    public:
        void construct(int argc, char* argv[]) override {
            TheCosmos::construct(argc, argv);
            this->set_window_size(1200, 0);
            
            // 按顺序加载各个任务类
            // 第一阶段
            this->push_plane(new ShapeWorld());
            this->push_plane(new PaddleBallWorld());
            this->push_plane(new Gallery());

            // 第二阶段

            // 第三阶段
            this->push_plane(new TheBigBang());
            this->push_plane(new TheBigBang());
            this->push_plane(new StreamPlane(this->stream_source.c_str()));
        }

    protected:
        void parse_cmdline_options(int argc, char* argv[]) override {
            CmdlineOps opt = CmdlineOps::_;
            
            for (int idx = 1; idx < argc; idx ++) {
                switch (opt) {
                    case CmdlineOps::StreamFile: {
                        this->stream_source = argv[idx];
                        opt = CmdlineOps::_;
                    }; break;
                    default: {
                        if (strncmp("--pipe", argv[idx], 7) == 0) {
                            opt = CmdlineOps::StreamFile;
                        }
                    }
                }
            }
        }
    private:
        std::string stream_source;
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
