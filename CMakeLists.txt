cmake_minimum_required(VERSION 3.9)
project(GLEE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

set(GLEE_SOURCE
    src/Dummy.cpp
    src/Drawing.cpp
    src/Drawing.hpp
    src/Window.cpp
    src/Window.hpp
    src/GLEE.hpp)

add_library(GLEE SHARED ${GLEE_SOURCE})
target_link_libraries(GLEE GL SDL2)

add_executable(testBasic test/testBasic.cpp)
target_link_libraries(testBasic GLEE)
