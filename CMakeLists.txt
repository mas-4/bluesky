cmake_minimum_required(VERSION 3.23)
project(skyblue)

set(CMAKE_CXX_STANDARD 20)

add_executable(skyblue main.cpp src/Config.cpp src/Config.h src/Renderer.cpp src/Renderer.h)
