#include "digitama/big_bang/game.hpp"

#include "digitama/big_bang/matter/graphlet/shapelet.hpp"
#include "digitama/big_bang/matter/graphlet/textlet.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    class Gallery : public World {
        public:
            Gallery() : World("The Gallery of Sprites and Graphlets") {}

        public:
            void load(float width, float height) override {
                float raft_width, raft_height;

                this->label = this->insert(new Labellet(game_font::unicode, BLACK, "精灵和图元陈列馆"));
                this->label->feed_extent(0.0F, 0.0F, &raft_width, &raft_height);

                raft_width *= 2.0F;
                raft_height *= 2.0F;

                this->sea = this->insert(new Ellipselet(raft_width * 1.618F, raft_height, DEEPSKYBLUE));
                
                this->mast = this->insert(new Rectanglet(4.0F, raft_width, BURLYWOOD, SADDLEBROWN));
                this->flag = this->insert(new Trianglet(-raft_height, raft_height * 0.618F, -raft_height, -raft_height * 0.618F, ROYALBLUE, DODGERBLUE));

                this->post = this->insert(new RoundedRectanglet(raft_height * 0.618F, raft_height * 2.0F, -0.45F, BURLYWOOD, BURLYWOOD));
                this->paddle = this->insert(new Linelet(raft_width * 0.618F, raft_height * 1.5F, BROWN));
                this->raft = this->insert(new RoundedRectanglet(raft_width, raft_height, -0.1F, BURLYWOOD, BURLYWOOD));
                this->bow = this->insert(new RegularPolygonlet(3, raft_height * 0.5F, 180.0F, KHAKI, BURLYWOOD));
                this->stern = this->insert(new RegularPolygonlet(3, raft_height * 0.5F, 0.0F, KHAKI, BURLYWOOD));
                
                this->caption = this->insert(new Labellet(game_font::DEFAULT, BLACK, this->get_renderer_name()));
            }

            void reflow(float width, float height) override {
                float raft_width, raft_height;

                this->label->feed_extent(0.0F, 0.0F, &raft_width, &raft_height);

                this->move_to(this->sea, width * 0.5F, height * 0.5F, MatterAnchor::CT);
                
                this->move_to(this->raft, this->sea, MatterAnchor::CT, MatterAnchor::CC);
                this->move_to(this->caption, this->raft, MatterAnchor::CC, MatterAnchor::CC);
                this->move_to(this->bow, this->raft, MatterAnchor::LC, MatterAnchor::RC);
                this->move_to(this->stern, this->raft, MatterAnchor::RC, MatterAnchor::LC);
                this->move_to(this->post, this->raft, MatterAnchor::RB, MatterAnchor::RB, -raft_height);
                this->move_to(this->paddle, this->post, MatterAnchor::CT, MatterAnchor::LT, -raft_height, raft_height);

                this->move_to(this->mast, this->raft, MatterAnchor::LB, MatterAnchor::LB, raft_height);
                this->move_to(this->flag, this->mast, MatterAnchor::RT, MatterAnchor::LT, 0.0F, raft_height * 0.5F);
            }

        public:
            bool can_select(WarGrey::STEM::IMatter* m) override {
                return true;
            }

        private: // 本游戏世界中的有以下物体
            WarGrey::STEM::Labellet* label;
            WarGrey::STEM::Labellet* caption;
            WarGrey::STEM::IShapelet* raft;
            WarGrey::STEM::IShapelet* bow;
            WarGrey::STEM::IShapelet* stern;
            WarGrey::STEM::IShapelet* flag;
            WarGrey::STEM::IShapelet* mast;
            WarGrey::STEM::IShapelet* post;
            WarGrey::STEM::IShapelet* paddle;
            WarGrey::STEM::IShapelet* sea;
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    Gallery universe;

    universe.construct(argc, args);
    universe.big_bang();

    return 0;
}
