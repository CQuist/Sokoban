cmake_minimum_required(VERSION 3.12)
project(SokobanSolver)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_library(hungarian hungarianMethod/hungarian.cpp hungarianMethod/hungarian.hpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(SokobanSolver main.cpp Tree.cpp Tree.h Point.cpp Point.h NodeV2.cpp NodeV2.h)

target_link_libraries(SokobanSolver hungarian)