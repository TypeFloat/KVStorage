cmake_minimum_required(VERSION 3.16)
project(SkipList)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)

include_directories(include)
aux_source_directory($src SRC)

add_subdirectory(test)
add_subdirectory(example)