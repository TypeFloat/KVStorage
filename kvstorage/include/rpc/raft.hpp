#pragma once

#include <memory>

#include "raft.grpc.pb.h"
#include "raft.pb.h"
#include "raft/peer.hpp"
#include "rpc/rpc.hpp"

class RaftPeer;

/**
 * @brief 服务端的函数处理逻辑
 *
 */
class RaftRpcService final : public RaftRpcProtoc::RaftRpc::Service {
   public:
    explicit RaftRpcService(RaftPeer* raftPeer) : raftPeer(raftPeer){};
    ~RaftRpcService() override = default;
    grpc::Status AppendEntries(grpc::ServerContext*,
                               const RaftRpcProtoc::AppendEntriesRequest*,
                               RaftRpcProtoc::AppendEntriesResponse*) override;
    grpc::Status RequestVote(grpc::ServerContext*,
                             const RaftRpcProtoc::RequestVoteRequest*,
                             RaftRpcProtoc::RequestVoteResponse*) override;
    grpc::Status InstallSnapshot(
        grpc::ServerContext*, const RaftRpcProtoc::InstallSnapshotRequest*,
        RaftRpcProtoc::InstallSnapshotResponse*) override;

   private:
    RaftPeer* raftPeer;
};

/**
 * @brief
 * 客户端实例，客户端负责填充请求参数，服务端回填响应，具体的gRPC调用由stub负责。
 *
 */
class RaftRpcClient : public RpcClient {
   public:
    RaftRpcClient(const std::string& host, const std::string& port)
        : RpcClient(host, port) {
        stub = RaftRpcProtoc::RaftRpc::NewStub(this->channel);
    }
    grpc::Status AppendEntries(const RaftRpcProtoc::AppendEntriesRequest &request,
                               RaftRpcProtoc::AppendEntriesResponse *response);
    grpc::Status RequestVote(const RaftRpcProtoc::RequestVoteRequest &request,
                             RaftRpcProtoc::RequestVoteResponse *response);
    grpc::Status InstallSnapshot(RaftRpcProtoc::InstallSnapshotRequest &request,
                                 RaftRpcProtoc::InstallSnapshotResponse *response);

   private:
    std::unique_ptr<RaftRpcProtoc::RaftRpc::Stub> stub;
};
