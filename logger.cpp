#include "logger.h"

using namespace std;

auto console_sink = make_shared<spdlog::sinks::stdout_color_sink_mt>();

shared_ptr<spdlog::logger> create_logger(string name)
{
    shared_ptr<spdlog::logger> logger = make_shared<spdlog::logger>(name, console_sink);
    logger->set_level(spdlog::level::trace);
    return logger;
}