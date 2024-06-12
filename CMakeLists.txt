cmake_minimum_required(VERSION 3.16)
project(SkipList)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
find_package(Threads REQUIRED)

include_directories(include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)

add_executable(main stress.cpp ${SRC})
target_link_libraries(main Threads::Threads)

add_subdirectory(test)