find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
aux_source_directory(. TEST)

add_executable(test_main ${TEST} ${SRC}
        cache.cpp)
target_link_libraries(test_main ${GTEST_LIBRARIES})

enable_testing()