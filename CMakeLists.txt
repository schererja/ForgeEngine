cmake_minimum_required(VERSION 3.20)

project(ForgeEngine
    VERSION 0.1.0
    DESCRIPTION "ForgeEngine - A 2D RPG Engine"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# FetchContent for dependencies
include(FetchContent)

FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.32.10
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.1
)
FetchContent_MakeAvailable(spdlog)

add_library(glad STATIC thirdparty/glad/src/gl.c)
target_include_directories(glad PUBLIC thirdparty/glad/include)

# cmrc
include(cmake/CMakeRC.cmake)

cmrc_add_resource_library(ForgeAssets
    ALIAS Forge::Assets
    NAMESPACE forge
    game/assets/shaders/sprite.vert
    game/assets/shaders/sprite.frag
)

add_subdirectory(engine)
add_subdirectory(plugins)
add_subdirectory(game)
add_subdirectory(tools)