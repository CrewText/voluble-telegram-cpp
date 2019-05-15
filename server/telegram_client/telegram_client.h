#ifndef TELEGRAM_CLIENT_H
#define TELEGRAM_CLIENT_H
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <random>
#include <functional>

#include "td/telegram/Client.h"
#include "td/telegram/Log.h"
#include "handlers/authorization_handler.h"
#include "handlers/option_handler.h"
#include "handlers/connectionstate_handler.h"
#include "spdlog/spdlog.h"

void fatal_error_callback(const char *error_message);

class TelegramClient
{
public:
    TelegramClient();
    virtual ~TelegramClient();
    void do_receive_loop();
    void send_query(td::td_api::object_ptr<td::td_api::Function> &fn, std::function<void(td::td_api::object_ptr<td::td_api::Object>)> handler);

private:
    std::shared_ptr<spdlog::logger> _logger;

    bool is_closed = false;

    std::shared_ptr<td::Client> _client;
    std::vector<td::td_api::object_ptr<td::td_api::OptionValue>> options;
    std::map<uint64_t, std::function<void(td::td_api::object_ptr<td::td_api::Object>)>> inflight_requests;
    AuthorizationHandler *authHandler;
    OptionHandler *optionHandler;
    ConnectionStateHandler *connectionStateHandler;

    void handle_td_update(td::Client::Response &response);
};

#endif