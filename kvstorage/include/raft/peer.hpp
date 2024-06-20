#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "raft.pb.h"
#include "rpc/raft.hpp"
#include "rpc/rpc.hpp"

class RaftRpcClient;

class RaftRpcService;

enum PeerStatus {
    Follower, Candidate, Leader
};
enum VoteStatus {
    Killed, Voted, Expire, Normal
};

class RaftPeer {
private:
    std::mutex
            mtx;  // 保证自己的各个属性在同一时刻只能被一个线程访问，因为在远程过程调用中，可能同时发生多个请求
    int idx;    // 当前节点在peers中的索引
    int term;   // 当前节点的任期
    int voteTo;   // 当前节点投票给了谁
    int voteNum;  // 当前节点的选票
    std::vector<RaftRpcProtoc::LogEntry>
            logs;  // 日志条目数组，包含状态机执行的指令集以及收到时的任期
    int lastCommitIndex;  // 已知的最大已提交的日志条目的索引
    int lastApplied;      // 已知的最大已应用的日志条目的索引
    int lastSnapshotIndex;  // 快照的最后一个日志条目的索引
    int lastSnapshotTerm;   // 快照的最后一个日志条目的任期
    PeerStatus status;        // 当前节点的状态
    std::chrono::system_clock::time_point electionTimeout;
    std::chrono::system_clock::time_point heartbeatTimeout;
    RpcServer rpcServer;

    void persist();

    // 选举相关功能
    static std::chrono::milliseconds getRandomElectionTimeout();

    void electionTimeoutTicker();

    void elect();

    std::pair<int, int> getLastIndexAndTerm();

public:
    RaftPeer(int, int peerNum);

    std::vector<std::shared_ptr<RaftRpcClient>>
            peers;  // 与其他Raft节点点对点通信的Rpc入口，每一个client负责连接对应peer的服务
    // 作为客户端的功能
    void sendRequestVote(int peerIdx, const RaftRpcProtoc::RequestVoteRequest &request);

    void sendAppendEntries();

    void sendInstallSnapshot();

    // 作为服务端的功能
    void receiveRequestVote(const RaftRpcProtoc::RequestVoteRequest *request,
                            RaftRpcProtoc::RequestVoteResponse *response);

    void receiveAppendEntries(
            const RaftRpcProtoc::AppendEntriesRequest *request,
            RaftRpcProtoc::AppendEntriesResponse *response);

    void receiveInstallSnapshot(
            const RaftRpcProtoc::InstallSnapshotRequest *request,
            RaftRpcProtoc::InstallSnapshotResponse *response);


    void init();
};