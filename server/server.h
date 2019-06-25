#ifndef SERVER_H
#define SERVER_H
// #include <cpprest/http_listener.h>
// #include <cpprest/json.h>
#include <iostream>
#include <string>
#include <vector>
#include "telegram_client/telegram_client.h"
#include "database_manager/database_manager.h"
#include "database_manager/database_exceptions.h"
#include <thread>
#include "../logger.h"

class Server
{
public:
    Server();
    ~Server();
    void handle_request();

private:
    // void handle_get(web::http::http_request request);
    std::vector<std::thread *> client_threads;
    DatabaseManager *db_manager;
    std::shared_ptr<spdlog::logger> _logger;
};
#endif