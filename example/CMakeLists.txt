add_executable(skiplist skiplist.cpp)
target_link_libraries(skiplist Threads::Threads Boost::serialization)