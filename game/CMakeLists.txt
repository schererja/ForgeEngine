# Collect game sources files
file(GLOB_RECURSE GAME_SOURCES
    "src/*.cpp"
    "src/*.h"
)
# Add the executable
add_executable(ForgeGame ${GAME_SOURCES})
# Include directories
target_include_directories(ForgeGame

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(ForgeGame
    PRIVATE
        ForgeEngine
)