#include "Config.h"

#include <sstream>

namespace Forge {
bool Config::load(const std::string& filename) {
    lua.open_libraries(sol::lib::base, sol::lib::string);

    auto result = lua.safe_script_file(filename, &sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;
        FORGE_ERROR("Failed to load config {}: {}", filename, err.what());
        return false;
    }
    loaded = true;
    FORGE_INFO("Config loaded from {}", filename);
    return true;
}

sol::object Config::resolve(const std::string& key) const {
    // Split key by '.'
    std::istringstream stream(key);
    std::string token;
    sol::object current = lua.globals();

    while (std::getline(stream, token, '.')) {
        if (!current.valid()) {
            return sol::lua_nil;
        }
        current = current.as<sol::table>()[token];
    }
    return current;
}

std::string Config::getString(const std::string& key,
                              const std::string& defaultValue) const {
    if (!loaded) {
        return defaultValue;
    }
    auto obj = resolve(key);
    if (!obj.valid() || obj.get_type() != sol::type::string) {
        FORGE_WARN(
            "Config key '{}' not found or not a string, using default '{}'",
            key, defaultValue);
        return defaultValue;
    }
    return obj.as<std::string>();
}

int Config::getInt(const std::string& key, int defaultValue) const {
    if (!loaded) {
        return defaultValue;
    }
    auto obj = resolve(key);
    if (!obj.valid() || obj.get_type() != sol::type::number) {
        FORGE_WARN(
            "Config key '{}' not found or not a number, using default '{}'",
            key, defaultValue);
        return defaultValue;
    }
    return obj.as<int>();
}

float Config::getFloat(const std::string& key, float defaultValue) const {
    if (!loaded) {
        return defaultValue;
    }
    auto obj = resolve(key);
    if (!obj.valid() || obj.get_type() != sol::type::number) {
        FORGE_WARN(
            "Config key '{}' not found or not a number, using default '{}'",
            key, defaultValue);
        return defaultValue;
    }
    return obj.as<float>();
}

bool Config::getBool(const std::string& key, bool defaultValue) const {
    if (!loaded) {
        return defaultValue;
    }
    auto obj = resolve(key);
    if (!obj.valid() || obj.get_type() != sol::type::boolean) {
        FORGE_WARN(
            "Config key '{}' not found or not a boolean, using default '{}'",
            key, defaultValue);
        return defaultValue;
    }
    return obj.as<bool>();
}

}  // namespace Forge