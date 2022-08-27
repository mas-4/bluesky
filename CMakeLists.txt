cmake_minimum_required(VERSION 3.23)
project(bluesky)

set(CMAKE_CXX_STANDARD 20)

add_executable(bluesky
        main.cpp
        src/Config.cpp
        src/Config.h
        src/Site.cpp
        src/Site.h
        src/Page.cpp
        src/Page.h
        src/Constants.h
        src/Constants.cpp
        src/Block.cpp
        src/Block.h
        src/utils.cpp
        src/utils.h)
