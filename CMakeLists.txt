cmake_minimum_required(VERSION 3.9)
project(GLEE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

set(GLEE_SOURCE
    src/Utils.hpp
    src/Utils.cpp
    src/drawing/LineBatch.hpp
    src/drawing/LineBatch.cpp
    src/drawing/Line.hpp
    src/drawing/Line.cpp
    src/drawing/Vertex2.hpp
    src/drawing/Vertex2.cpp
    src/Color.hpp
    src/Window.cpp
    src/Window.hpp
    src/EventHandlers.hpp
    src/GLEE.cpp
    src/GLEE.hpp)

add_library(GLEE SHARED ${GLEE_SOURCE})
target_link_libraries(GLEE GL GLEW SDL2)

add_executable(testBasic test/testBasic.cpp)
target_link_libraries(testBasic GLEE)

add_executable(testVAO test/testVAO.cpp)
target_link_libraries(testVAO GLEE)

add_executable(test2 test/text2.cpp)
target_link_libraries(test2 GL GLEW SDL2)
