project("octree")

cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Qt5 COMPONENTS Widgets DataVisualization 3DCore 3DRender 3DInput 3DExtras REQUIRED)

file(GLOB SRCFILES
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(octree_vis ${SRCFILES})

target_link_libraries(
    octree_vis
    Qt5::Widgets
    Qt5::DataVisualization
    Qt5::3DCore
    Qt5::3DExtras
    Qt5::3DRender
    Qt5::3DInput
)

