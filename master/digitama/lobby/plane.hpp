#include <gydm_stem/game.hpp>

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace WarGrey::STEM {
    class LobbyPlane : public Plane {
    public:
        LobbyPlane() : Plane("青少计算机科学") {}

    public:  // 覆盖游戏基本方法
        void load(float width, float height) override;
        void reflow(float width, float height) override;
        void update(uint64_t count, uint32_t interval, uint64_t uptime) override;

    public:
        bool can_select(IMatter* m) override { return true; }

    protected:
        bool update_tooltip(IMatter* m, float local_x, float local_y, float global_x, float global_y) override;

    private:
        Linkmon* agent;
        Labellet* title;
        Labellet* tooltip;
    };
}
