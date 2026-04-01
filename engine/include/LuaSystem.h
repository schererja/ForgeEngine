#pragma once

#include <filesystem>
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>

#include "Log.h"
namespace Forge {

class Engine;

class LuaSystem {
   public:
    LuaSystem() = default;
    ~LuaSystem() = default;

    LuaSystem(const LuaSystem&) = delete;
    LuaSystem& operator=(const LuaSystem&) = delete;

    bool initialize(Engine& engine);
    void shutdown();

    // Load and execute a Lua script
    bool loadScript(const std::string& path);

    // Call a Lua function by name
    template <typename... Args>
    void callFunction(const std::string& name, Args&&... args) {
        sol::protected_function func = lua[name];
        if (func.valid()) {
            auto result = func(std::forward<Args>(args)...);
            if (!result.valid()) {
                sol::error err = result;
                FORGE_ERROR("Lua error in {}: {}", name, err.what());
            }
        }
    };

    // Get a value from Lua
    template <typename T>
    T get(const std::string& key) {
        return lua[key].get<T>();
    };

    // Set a value in Lua
    template <typename T>
    void set(const std::string& key, T value) {
        lua[key] = value;
    };

    // Hot reload - check if any watched scripts changed
    void checkHotReload();

    // Watch a script for changes
    void watchScript(const std::string& path);

    // Direct access to sol state for bindings
    sol::state& getState() { return lua; }

   private:
    sol::state lua;
    bool initialized = false;

    // Hot reload tracking
    std::unordered_map<std::string, std::filesystem::file_time_type>
        watchedFiles;

    void bindEngineAPI(Engine& engine);
    void reloadScript(const std::string& path);
};

}  // namespace Forge