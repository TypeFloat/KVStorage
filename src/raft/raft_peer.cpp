#include "raft/raft_peer.hpp"

#include <memory>

bool RaftPeer::sendRequestVote(int server,
                               std::shared_ptr<maprpc::RequestVoteArgs> args,
                               std::shared_ptr<mprrpc::RequestVoteReply> reply,
                               std::shared_ptr<int> votedNum) {}