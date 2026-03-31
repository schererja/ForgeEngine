#pragma once

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace Forge {
class Log {
   public:
    static void init();
    static void shutdown();
    static std::shared_ptr<spdlog::logger>& getLogger() { return logger; }

   private:
    static std::shared_ptr<spdlog::logger> logger;
};
}  // namespace Forge

// Macro for easy logging
#define FORGE_TRACE(...) ::Forge::Log::getLogger()->trace(__VA_ARGS__)
#define FORGE_DEBUG(...) ::Forge::Log::getLogger()->debug(__VA_ARGS__)
#define FORGE_INFO(...) ::Forge::Log::getLogger()->info(__VA_ARGS__)
#define FORGE_WARN(...) ::Forge::Log::getLogger()->warn(__VA_ARGS__)
#define FORGE_ERROR(...) ::Forge::Log::getLogger()->error(__VA_ARGS__)
#define FORGE_CRITICAL(...) ::Forge::Log::getLogger()->critical(__VA_ARGS__)
