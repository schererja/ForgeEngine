# Find SDL2

find_package(OpenGL REQUIRED)
#collect engine source files

file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
    "include/*.h"
)

#add the executable
add_library(ForgeEngine STATIC ${ENGINE_SOURCES})

target_include_directories(ForgeEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/thirdparty/stb
        ${CMAKE_SOURCE_DIR}/thirdparty/miniaudio
)
# Link SDL2
target_link_libraries(ForgeEngine
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        glad
        OpenGL::GL
        glm::glm
        Forge::Assets
        spdlog::spdlog
)
