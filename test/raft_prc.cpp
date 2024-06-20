#include <gtest/gtest.h>
#include "raft.grpc.pb.h"
#include "raft.pb.h"
#include "rpc/raft.hpp"

class TestRaftRpcService final : public RaftRpcProtoc::RaftRpc::Service {
    grpc::Status RequestVote(grpc::ServerContext* context,
                             const RaftRpcProtoc::RequestVoteRequest* request,
                             RaftRpcProtoc::RequestVoteResponse* response) {
        response->set_votegranted(true);
        response->set_term(10);
        response->set_votestate(VoteStatus::Normal);
        return grpc::Status::OK;
    }
};

TEST(RaftRpc, Client) {
    int pid = fork();
    if (pid > 0) {
        sleep(1);
        RaftRpcClient client("localhost", "2510");
        RaftRpcProtoc::RequestVoteRequest request;
        RaftRpcProtoc::RequestVoteResponse response;
        client.RequestVote(request, &response);
        ASSERT_EQ(response.term(), 10);
        ASSERT_EQ(response.votegranted(), true);
        ASSERT_EQ(response.votestate(), VoteStatus::Normal);
    } else if (pid == 0) {
        std::shared_ptr<TestRaftRpcService> service = std::make_shared<TestRaftRpcService>();
        RpcServer server("localhost", "2510", service);
        server.run();
    }
}

TEST(RaftPeer, RaftPeer) {
    int pid = fork();
    if (pid > 0) {
        sleep(1);
        RaftPeer peer(0, 5);
        RaftRpcProtoc::RequestVoteRequest request;
        RaftRpcProtoc::RequestVoteResponse response;
        peer.peers[2]->RequestVote(request, &response);
        ASSERT_EQ(response.term(), 10);
        ASSERT_EQ(response.votegranted(), true);
        ASSERT_EQ(response.votestate(), VoteStatus::Normal);
    }
    else if (pid == 0) {
        std::shared_ptr<TestRaftRpcService> service = std::make_shared<TestRaftRpcService>();
        RpcServer server("localhost", "5002", service);
        server.run();
    }
}