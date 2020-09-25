project("octree")

cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB PRSRCFILES
    "pr-octree/src/*.cpp"
    "pr-octree/src/*.hpp"
)

file(GLOB REGIONSRCFILES
    "region-octree/src/*.cpp"
    "region-octree/src/*.hpp"
)

add_executable(pr_octree ${PRSRCFILES})
add_executable(region_octree ${REGIONSRCFILES})

