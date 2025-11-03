cmake_minimum_required(VERSION 3.16)
project(MazeUtil VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")
add_library(MazeUtilLib ${SOURCES})
target_include_directories(MazeUtilLib PUBLIC ${PROJECT_SOURCE_DIR}/include/mazeutil)

add_executable(MazeUtil src/main.cpp
        include/mazeutil/hash.h)
target_link_libraries(MazeUtil PRIVATE MazeUtilLib)

if(MSVC)
    target_compile_options(MazeUtil PRIVATE /W4 /permissive-)
else()
    target_compile_options(MazeUtil PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -O2)
endif()
