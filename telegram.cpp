#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
// #include "spdlog/sinks/stdout_sinks.h"
#include "VolubleTelegramClientConfig.h"
#include "server/server.h"

using namespace std;

int main()
{
    cout
        << "VTH Version: "
        << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH
        << endl;

    auto logger = spdlog::stdout_color_mt("console");
    logger->set_level(spdlog::level::trace);
    logger->info("Created logger");
    handle_request();

    return 0;
}