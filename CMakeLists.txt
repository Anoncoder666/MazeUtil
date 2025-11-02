cmake_minimum_required(VERSION 3.16)
project(MazeUtil VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directory for headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Automatically find all .cpp files inside src/
file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS src/*.cpp)

# If you have a main.cpp file, make an executable from it
# and link the rest as a library
set(MAIN_FILE src/main.cpp)
if(EXISTS ${MAIN_FILE})
    list(REMOVE_ITEM ALL_SRC ${MAIN_FILE})
    add_library(MazeUtilLib ${ALL_SRC})
    target_include_directories(MazeUtilLib PUBLIC ${PROJECT_SOURCE_DIR}/include/mazeutil)

    add_executable(MazeUtil ${MAIN_FILE})
    target_link_libraries(MazeUtil PRIVATE MazeUtilLib)
else()
    # Fallback: just build everything as one executable
    add_executable(MazeUtil ${ALL_SRC})
endif()
