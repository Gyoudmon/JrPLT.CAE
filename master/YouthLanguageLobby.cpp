#include <gydm_stem/game.hpp>
#include <gydm_stem/datum/time.hpp>

#include <gydm_stem/network.hpp>

#include "digitama/lobby/plane.hpp"

#include <vector>
#include <filesystem>

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    enum class CmdlineOps { _ };

    class YouthLanguageCosmos : public Cosmos, public UDPLocalPeer<CmdlineOps> {
    public:
        YouthLanguageCosmos(const char* process_path) : Cosmos(60) {
            enter_digimon_zone(process_path);
            digimon_mascot_setup("../mascot");
            imgdb_setup(digimon_subdir("stone"));
        }

        virtual ~YouthLanguageCosmos() {
            imgdb_teardown();
        }

    public:  // 覆盖游戏基本方法
        void construct(int argc, char* argv[]) override {
            this->set_window_size(1200, 0);
            GameFont::fontsize(21);

            this->daemon.udp_listen(this, 2333);
            this->daemon.start_wait_read_process_loop(0);

            this->push_plane(new LobbyPlane());
        }

    protected:
        void update(uint64_t count, uint32_t interval, uint64_t uptime) override {
            if (this->has_current_mission_completed()) {
                this->transfer_to_plane(0);
            }
        }

        void on_user_datagram(uint16_t service, CmdlineOps type, const shared_datagram_t& datagram) override {
            printf("[%u]%s says: %s\n", datagram->transaction, datagram->remote_host,
                    std::basic_string<unsigned char>(datagram->payload, datagram->payload_size).c_str());
        }

    private:
        SocketDaemon daemon;
    };
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    YouthLanguageCosmos universe(argv[0]);

    universe.construct(argc, argv);
    universe.big_bang();

    return 0;
}
