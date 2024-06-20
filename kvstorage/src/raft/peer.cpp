#include "raft/peer.hpp"

#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <utility>
#include <glog/logging.h>

#include "raft.grpc.pb.h"
#include "raft.pb.h"
#include "common/config.hpp"

RaftPeer::RaftPeer(int idx, int peerNum) : idx(idx), term(0), voteTo(-1), voteNum(0), lastSnapshotTerm(0),
                                           lastSnapshotIndex(0),
                                           lastApplied(0), lastCommitIndex(0), status(PeerStatus::Follower),
                                           rpcServer(HOST, std::to_string(MIN_PORT + idx),
                                                     std::make_shared<RaftRpcService>(this)) {
    this->status = PeerStatus::Follower;
    for (int i = 0; i < peerNum; i++) {
        this->peers.push_back(std::make_shared<RaftRpcClient>(HOST, std::to_string(MIN_PORT + i)));
    }
    LOG(INFO) << "RaftPeer " << idx << " is created successfully.";
}

void RaftPeer::init() {
    std::thread serverThread = std::thread(&RpcServer::run, &this->rpcServer);
    serverThread.detach();
    this->electionTimeout = std::chrono::system_clock::now();
    this->heartbeatTimeout = std::chrono::system_clock::now();
    std::thread electionThread = std::thread(&RaftPeer::electionTimeoutTicker, this);
    electionThread.detach();
    LOG(INFO) << "Finish initializing.";
}

/**
 * @brief
 * 向某个peerIdx发送RequestVote请求（请求头需要在调用前填充好），并处理返回结果。
 * 主要有以下几种情况：
 *
 * 1. 对方term比自己大，说明自己已经落后，退出选举成为追随者并更新term；
 * 2. 对方term比自己小，说明对方已经落后，忽略响应；
 * 3.
 * 对方term和自己一样，根据votegranted判断是否收到投票，如果收到投票则投票数加1。
 */
void RaftPeer::sendRequestVote(int peerIdx, const RaftRpcProtoc::RequestVoteRequest &request) {
    LOG(INFO) << "Send RequestVote to " << peerIdx << ", current term is " << this->term << ".";
    RaftRpcProtoc::RequestVoteResponse response;
    grpc::Status rpcStatus =
            this->peers[peerIdx]->RequestVote(request, &response);
    if (!rpcStatus.ok()) {
        LOG(ERROR) << "RPC failed: " << rpcStatus.error_message();
        return;
    }
    LOG(INFO) << "Receive RequestVote response from " << peerIdx << ".";
    std::lock_guard<std::mutex> localMtx(this->mtx);
    // 已经在选举中当选了leader，不需要再处理投票
    if (this->status == PeerStatus::Leader) return;
    // NOTE: 任何时候，只要请求中的任期大于自己的，就说明自己已经落后，需要更新自己的任期以及状态。
    if (response.term() > this->term) {
        this->status = PeerStatus::Follower;
        this->term = response.term();
        this->voteTo = -1;
        // TODO 搞清楚persist的功能和作用
        persist();
        return;
    } else if (response.term() < this->term)
        return;
    else {
        if (!response.votegranted()) {
            LOG(INFO) << "Receive negative vote from " << peerIdx << ".";
            return;
        } else {
            LOG(INFO) << "Receive positive vote from " << peerIdx << ".";
            ++(this->voteNum);
        }
        if (this->voteNum >= this->peers.size() / 2 + 1) {
            this->voteNum = 0;
            this->status = PeerStatus::Leader;
            LOG(INFO) << "Become a leader.";
        }
        return;
    }
}

/**
 * @brief 服务端处理请求的逻辑：
 *
 * 1. 如果请求方的任期小于自己的，说明对方落后，投否定票，原因是竞选者落后，即Expire；
 * 2. 如果请求方的任期大于等于自己的，要判断日志记录并给出投票，如果竞选者的日志更新且没有投票，则同意，否则不同意。
 */
void RaftPeer::receiveRequestVote(
        const RaftRpcProtoc::RequestVoteRequest *request,
        RaftRpcProtoc::RequestVoteResponse *response) {
    LOG(INFO) << "Receive RequestVote from " << request->candidateid() << ".";
    std::lock_guard localMtx(this->mtx);
    this->persist();
    if (request->term() < this->term) {
        response->set_term(this->term);
        response->set_votestate(VoteStatus::Expire);
        response->set_votegranted(false);
        return;
    }

    // NOTE: 任何时候，只要请求中的任期大于自己的，就说明自己已经落后，需要更新自己的任期以及状态。
    if (request->term() > this->term) {
        this->term = request->term();
        this->status = PeerStatus::Follower;
        this->voteTo = -1;
    }
    // 当前任期已经一致，判断日志情况，如果对方日志不比自己新的话，投否定票，原因是对方日志落后，即Expire；
    std::pair<int, int> indexAndItem = this->getLastIndexAndTerm();
    int lastLogIndex = indexAndItem.first;
    int lastLogTerm = indexAndItem.second;
    if (request->lastlogindex() < lastLogIndex ||
        (request->lastlogindex() == lastLogIndex &&
         request->lastlogterm() < lastLogTerm)) {
        response->set_term(this->term);
        response->set_votestate(VoteStatus::Expire);
        response->set_votegranted(false);
        return;
    }

    // 避免因为网络波动造成的重复投票，需进行判断
    if (this->voteTo != -1 && this->voteTo != request->candidateid()) {
        response->set_term(this->term);
        response->set_votestate(VoteStatus::Voted);
        response->set_votegranted(false);
        return;
    } else {
        this->voteTo = request->candidateid();
        response->set_term(this->term);
        response->set_votestate(VoteStatus::Normal);
        response->set_votegranted(true);
        // 完成投票后重置自己的选举过期时间
        this->electionTimeout = std::chrono::system_clock::now();
        return;
    }
}

std::chrono::milliseconds RaftPeer::getRandomElectionTimeout() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(MIN_RANDOM_ELECTION_TIMEOUT,
                                            MAX_RANDOM_ELECTION_TIMEOUT);
    return std::chrono::milliseconds(dist(rng));
}

void RaftPeer::electionTimeoutTicker() {
    // 作为一个单独的线程，不断的检查选举超时定时器
    while (true) {
        mtx.lock();
        std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();
        // TODO 参数化
        // 检查是否超时，大于0就代表没有超时
        std::chrono::system_clock::duration suitableSleepTime =
                RaftPeer::getRandomElectionTimeout() - (nowTime - this->electionTimeout);
        mtx.unlock();
        if (suitableSleepTime.count() > 0)
            std::this_thread::sleep_for(suitableSleepTime);
        // 如果条件判断成立，说明在睡眠期间electionTimeout被重置了，不需要进行选举
        mtx.lock();
        std::chrono::system_clock::time_point timeOut = this->electionTimeout;
        mtx.unlock();
        if ((timeOut - nowTime).count() > 0) continue;
        else {
            LOG(INFO) << "Election timeout, start to elect.";
            this->elect();
        }
    }
}

void RaftPeer::elect() {
    std::lock_guard<std::mutex> localMtx(this->mtx);
    if (this->status == PeerStatus::Leader) return;
    // 不是leader，开始进行选举
    this->electionTimeout =
            std::chrono::system_clock::now();  // 重置选举超时定时器
    this->status = PeerStatus::Candidate;
    this->term++;
    this->voteTo = this->idx;  // 投票给自己
    // TODO 搞清楚persist的功能和作用
//    this->persist();
    std::thread thread;
    // 向其他节点发送投票请求
    for (int i = 0; i < this->peers.size(); i++) {
        if (i == this->idx) continue;
        std::pair<int, int> indexAndItem = this->getLastIndexAndTerm();
        int lastLogIndex = indexAndItem.first;
        int lastLogTerm = indexAndItem.second;
        // 向第i个peer发送投票请求
        // 节点在拉票时，需要告诉其他节点当前任期和最后一条日志的索引和任期
        RaftRpcProtoc::RequestVoteRequest request;
        request.set_term(this->term);
        request.set_candidateid(this->idx);
        request.set_lastlogterm(lastLogIndex);
        request.set_lastlogindex(lastLogTerm);
        // 多线程请求，这样才是节点同时向其他节点发送请求
        thread = std::thread(&RaftPeer::sendRequestVote, this, i, request);
        thread.detach();
    }
}

std::pair<int, int> RaftPeer::getLastIndexAndTerm() {
    if (this->logs.empty())
        return std::make_pair(this->lastSnapshotIndex, this->lastSnapshotTerm);
    else
        return std::make_pair(this->logs[this->logs.size() - 1].logindex(),
                              this->logs[this->logs.size() - 1].logterm());
}

void RaftPeer::persist() {

}

void RaftPeer::receiveAppendEntries(const RaftRpcProtoc::AppendEntriesRequest *request,
                                    RaftRpcProtoc::AppendEntriesResponse *response) {

}

void RaftPeer::receiveInstallSnapshot(const RaftRpcProtoc::InstallSnapshotRequest *request,
                                      RaftRpcProtoc::InstallSnapshotResponse *response) {

}

void RaftPeer::sendAppendEntries() {

}
