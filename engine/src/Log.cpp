#include "Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <vector>

namespace Forge {
std::shared_ptr<spdlog::logger> Log::logger;

void Log::init() {
    std::vector<spdlog::sink_ptr> sinks;

    // Console sink with color
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("[%T] [%^%l%$] %v");
    sinks.push_back(consoleSink);

    // File sink
    auto fileSink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("forge.log", true);
    fileSink->set_pattern("[%Y-%m-%d %T] [%l] %v");
    sinks.push_back(fileSink);
    // Create logger with both sinks
    logger =
        std::make_shared<spdlog::logger>("FORGE", sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);

    spdlog::register_logger(logger);
    FORGE_INFO("Logger initialized.");
}

void Log::shutdown() {
    FORGE_INFO("Shutting down logger.");
    spdlog::drop_all();
    spdlog::shutdown();
}
}  // namespace Forge