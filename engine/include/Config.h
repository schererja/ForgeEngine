#pragma once

#include <sol/sol.hpp>
#include <string>

#include "Log.h"

namespace Forge {

class Config {
   public:
    Config() = default;
    ~Config() = default;

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    // Load config from a Lua file
    bool load(const std::string& path);

    // Getters with defaults
    std::string getString(const std::string& key,
                          const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    float getFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;

    bool isLoaded() const { return loaded; }

   private:
    sol::state lua;
    bool loaded = false;

    // Resolve nested key like "window.width"
    // into lua["window"]["width"]
    sol::object resolve(const std::string& key) const;
};

}  // namespace Forge