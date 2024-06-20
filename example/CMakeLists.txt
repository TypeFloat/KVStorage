add_executable(raft raft.cpp ${PROTO} ${RPC} ${RAFT})
target_link_libraries(raft gRPC::grpc++_reflection gRPC::grpc++ protobuf::libprotobuf glog::glog)