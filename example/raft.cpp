#include <memory>
#include "raft/peer.hpp"
#include "common/config.hpp"
#include <glog/logging.h>

int main(int argc, char **argv) {

    google::InitGoogleLogging(argv[0]);
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <peerIdx>" << std::endl;
        return 1;
    }
    RaftPeer peer = RaftPeer(atoi(argv[1]), PEER_NUM);
    peer.init();
    while (1);
}