#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "raft_rpc.hpp"

enum Status { Follower, Candidate, Leader };

class RaftPeer {
   private:
    std::mutex mtx;
    std::vector<std::shared_ptr<RaftRpc>>
        peers;  // 与其他Raft节点点对点通信的Rpc入口
    int idx;    // 当前节点在peers中的索引
    int term;   // 当前节点的任期
    int voted;  // 当前节点投票给了谁
    // std::vector<mprrpc::LogEntry>
    // logs;  // 日志条目数组，包含状态机执行的指令集以及收到时的任期
    int commitIndex;  // 已知的最大已提交的日志条目的索引
    int lastApplied;  // 已知的最大已应用的日志条目的索引
    Status status;    // 当前节点的状态
   public:
    bool sendRequestVote();
};