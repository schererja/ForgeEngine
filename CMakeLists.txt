cmake_minimum_required(VERSION 3.20)

project(ForgeEngine 
    VERSION 0.1.0
    DESCRIPTION "Forge Engine is a 2d game engine written in C++ using SDL2 and OpenGL."
    LANGUAGES CXX
    )

# Require c++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# output binaries to a "bin" directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Add subdirectories
add_subdirectory(engine)
add_subdirectory(plugins)
add_subdirectory(game)
add_subdirectory(tools)
