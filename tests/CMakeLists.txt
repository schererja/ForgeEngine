# Collect test sources
file(GLOB_RECURSE TEST_SOURCES
    "src/*.cpp"
)

add_executable(ForgeTests ${TEST_SOURCES})

target_include_directories(ForgeTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ForgeTests
    PRIVATE
        ForgeEngine
        Catch2::Catch2WithMain
)

# Register tests with CTest
include(CTest)
include(Catch)
catch_discover_tests(ForgeTests)