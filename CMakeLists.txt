project("octree")

cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SRCFILES
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(octree ${SRCFILES})

