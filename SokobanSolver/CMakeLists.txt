cmake_minimum_required(VERSION 3.12)
project(SokobanSolver)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(SokobanSolver main.cpp Tree.cpp Tree.h)
