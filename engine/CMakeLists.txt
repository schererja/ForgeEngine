# Find SDL2
find_package(SDL2 CONFIG REQUIRED)

#collect engine source files

file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
    "src/*.h"
)

#add the executable
add_executable(ForgeEngine ${ENGINE_SOURCES})

target_include_directories(ForgeEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
# Link SDL2
target_link_libraries(ForgeEngine
    PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
)