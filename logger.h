#ifndef LOGGER_H
#define LOGGER_H
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>

std::shared_ptr<spdlog::logger> create_logger(std::string name);