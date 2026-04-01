# ForgeEngine

A modern C++20 2D RPG engine and sample game built with SDL2, OpenGL, Lua scripting, and an ECS-style architecture.

## Highlights

- C++20 core engine with clear separation between engine and game code
- SDL2 windowing and input
- OpenGL rendering through GLAD
- Resource embedding for shaders using cmrc
- Lua integration through sol2
- Audio subsystem for music and effects
- Scene system with an example overworld scene
- Tilemap loading and collision helpers
- Catch2-based unit tests

## Tech Stack

- CMake 3.20+
- C++20
- SDL2
- OpenGL 3.3 + GLAD
- glm
- spdlog
- Lua 5.4
- sol2
- Catch2

All major dependencies are pulled automatically with CMake FetchContent.

## Project Structure

- engine: Engine systems, rendering, input, ECS, scripting, audio, map support
- game: Playable game code and assets
- tests: Unit tests for engine components
- thirdparty: GLAD and stb headers
- cmake: Helper scripts including resource embedding
- tools/plugins: Extension points for tooling and plugin experiments

## Build

1. Configure

   cmake -S . -B build

2. Build

   cmake --build build -j

## Run

From the repository root:

./build/bin/ForgeGame

The sample game boots into the overworld scene and loads assets from the game assets directory.

## Test

Run all tests:

ctest --test-dir build --output-on-failure

Or run the test binary directly:

./build/bin/ForgeTests

## Configuration and Assets

The game scene reads asset-related paths from engine config keys such as:

- paths.assets
- paths.audio
- paths.maps

Default map loading uses a Lua file in game assets maps.

## Development Notes

- The project exports compile_commands.json for accurate editor IntelliSense and clangd support.
- If editor diagnostics look stale, reconfigure the build directory:

  cmake -S . -B build

- Then reload your editor window or restart your language server.

## Roadmap Ideas

- Animation state machines and sprite atlases
- Save/load game state
- Better asset hot reload pipeline
- UI system and debug overlays
- Packaging workflow for assets and binaries

## License

Add your preferred license here, for example MIT, Apache-2.0, or GPL-3.0.
