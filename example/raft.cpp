#include <memory>
#include "raft/peer.hpp"
#include "common/config.hpp"
#include <glog/logging.h>

class TestRaftRpcService final : public RaftRpcProtoc::RaftRpc::Service {
    grpc::Status RequestVote(grpc::ServerContext *context,
                             const RaftRpcProtoc::RequestVoteRequest *request,
                             RaftRpcProtoc::RequestVoteResponse *response) {
        response->set_votegranted(true);
        response->set_term(10);
        response->set_votestate(VoteStatus::Normal);
        return grpc::Status::OK;
    }
};

int main(int argc, char **argv) {

    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = true;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <peerIdx>" << std::endl;
        return 1;
    }
    RaftPeer peer = RaftPeer(atoi(argv[1]), PEER_NUM);
    peer.init();
    while (1);
}