#include "rpc/raft.hpp"

#include "raft.pb.h"

grpc::Status RaftRpcService::AppendEntries(
    grpc::ServerContext* context,
    const RaftRpcProtoc::AppendEntriesRequest* request,
    RaftRpcProtoc::AppendEntriesResponse* response) {
    this->raftPeer->receiveAppendEntries(request, response);
    return grpc::Status::OK;
}

grpc::Status RaftRpcService::RequestVote(
    grpc::ServerContext* context,
    const RaftRpcProtoc::RequestVoteRequest* request,
    RaftRpcProtoc::RequestVoteResponse* response) {
    this->raftPeer->receiveRequestVote(request, response);
    return grpc::Status::OK;
}

grpc::Status RaftRpcService::InstallSnapshot(
    grpc::ServerContext* context,
    const RaftRpcProtoc::InstallSnapshotRequest* request,
    RaftRpcProtoc::InstallSnapshotResponse* response) {
    this->raftPeer->receiveInstallSnapshot(request, response);
    return grpc::Status::OK;
}

grpc::Status RaftRpcClient::AppendEntries(const RaftRpcProtoc::AppendEntriesRequest &request,
                                          RaftRpcProtoc::AppendEntriesResponse *response) {
    grpc::ClientContext context;
    return this->stub->AppendEntries(&context, request, response);
}

grpc::Status RaftRpcClient::RequestVote(const RaftRpcProtoc::RequestVoteRequest &request,
                                        RaftRpcProtoc::RequestVoteResponse *response) {
    grpc::ClientContext context;
    return this->stub->RequestVote(&context, request, response);
}

grpc::Status RaftRpcClient::InstallSnapshot(RaftRpcProtoc::InstallSnapshotRequest &request,
                                            RaftRpcProtoc::InstallSnapshotResponse *response) {
    grpc::ClientContext context;
    return this->stub->InstallSnapshot(&context, request, response);
}