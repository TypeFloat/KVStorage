find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories("calculator")
aux_source_directory(. TEST)
file(GLOB RPC_PROTO "calculator/*.pb.cc")

add_executable(test_main ${TEST} ${RPC} ${PROTO} ${RPC_PROTO} ${RAFT})
target_link_libraries(test_main ${GTEST_LIBRARIES} Boost::serialization gRPC::grpc++ protobuf::libprotobuf glog::glog)
enable_testing()