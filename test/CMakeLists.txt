find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
aux_source_directory(. TEST)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)

add_executable(test_main ${TEST} ${SRC})
target_link_libraries(test_main ${GTEST_LIBRARIES} Boost::serialization)

enable_testing()