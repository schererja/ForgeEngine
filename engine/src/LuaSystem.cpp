#include "LuaSystem.h"

#include <filesystem>

#include "ForgeEngine.h"
#include "Log.h"

namespace Forge {

bool LuaSystem::initialize(Engine& engine) {
    // Open standard Lua libraries
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string,
                       sol::lib::table, sol::lib::io, sol::lib::os);

    // Bind engine API to Lua
    bindEngineAPI(engine);

    initialized = true;
    FORGE_INFO("Lua system initialized.");
    return true;
}

void LuaSystem::shutdown() {
    if (!initialized) return;
    watchedFiles.clear();
    initialized = false;
    FORGE_INFO("Lua system shutdown.");
}

bool LuaSystem::loadScript(const std::string& path) {
    auto result = lua.safe_script_file(path, sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;
        FORGE_ERROR("Failed to load Lua script {}: {}", path, err.what());
        return false;
    }
    FORGE_INFO("Loaded Lua script: {}", path);
    watchScript(path);
    return true;
}

void LuaSystem::watchScript(const std::string& path) {
    try {
        watchedFiles[path] = std::filesystem::last_write_time(path);
    } catch (const std::exception& e) {
        FORGE_WARN("Could not watch script {}: {}", path, e.what());
    }
}

void LuaSystem::checkHotReload() {
    for (auto& [path, lastTime] : watchedFiles) {
        try {
            auto currentTime = std::filesystem::last_write_time(path);
            if (currentTime != lastTime) {
                lastTime = currentTime;
                reloadScript(path);
            }
        } catch (const std::exception& e) {
            FORGE_WARN("Hot reload check failed for {}: {}", path, e.what());
        }
    }
}

void LuaSystem::reloadScript(const std::string& path) {
    FORGE_INFO("Hot reloading: {}", path);
    auto result = lua.safe_script_file(path, sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;
        FORGE_ERROR("Hot reload failed for {}: {}", path, err.what());
    }
}

void LuaSystem::bindEngineAPI(Engine& engine) {
    // Audio bindings
    lua.set_function("playMusic",
                     [&engine](const std::string& path, float volume) {
                         engine.getAudio().playMusic(path, volume);
                     });

    lua.set_function("stopMusic",
                     [&engine]() { engine.getAudio().stopMusic(); });

    lua.set_function("playSound", [&engine](const std::string& path) {
        engine.getAudio().playSoundEffect(path);
    });

    lua.set_function("setMasterVolume", [&engine](float volume) {
        engine.getAudio().setMasterVolume(volume);
    });

    // Logging from Lua
    lua.set_function("log_info", [](const std::string& msg) {
        FORGE_INFO("[Lua] {}", msg);
    });
    lua.set_function("log_warn", [](const std::string& msg) {
        FORGE_WARN("[Lua] {}", msg);
    });
    lua.set_function("log_error", [](const std::string& msg) {
        FORGE_ERROR("[Lua] {}", msg);
    });

    FORGE_INFO("Engine API bound to Lua.");
}

}  // namespace Forge