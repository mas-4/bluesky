cmake_minimum_required(VERSION 3.23)
project(bluesky)

set(CMAKE_CXX_STANDARD 20)

set(MD4C tpc/md4c-release-0.4.8)

add_subdirectory(${MD4C}/src)

add_executable(bluesky
        main.cpp
        src/Config.cpp
        src/Site.cpp
        src/Page.cpp
        src/Constants.cpp
        src/Block.cpp
        src/utils.cpp
        src/Template.cpp
        src/Meta.cpp
        src/Meta.h)

target_link_libraries(bluesky md4c)
target_include_directories(bluesky PUBLIC ${MD4C}/src)