cmake_minimum_required(VERSION 3.7...3.24)
project(bluesky)

set(CMAKE_CXX_STANDARD 20)

set(MD4C tpc/md4c)

set(CXX_SRC main.cpp
        src/Config.cpp
        src/Site.cpp
        src/Page.cpp
        src/Constants.cpp
        src/Block.cpp
        src/utils.cpp
        src/Template.cpp
        src/Meta.cpp
        src/Markdown.cpp
        src/Logger.cpp
        ${MD4C}/entity.c
        ${MD4C}/md4c.c
        ${MD4C}/md4c-html.c src/Server.cpp src/Server.h)


add_executable(bluesky ${CXX_SRC})


target_include_directories(bluesky PUBLIC ${MD4C})