cmake_minimum_required(VERSION 3.9)
project(GLEE)

set(GLEE_SOURCE src/Dummy.cpp)

set(CMAKE_CXX_STANDARD 11)

add_library(GLEE SHARED ${GLEE_SOURCE})
target_link_libraries(GLEE GL SDL2)
