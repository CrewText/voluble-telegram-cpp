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

void fatal_error_callback(const char *error_message);

class TelegramClient
{
public:
    TelegramClient();
    void do_receive_loop();
    void send_query(td::td_api::object_ptr<td::td_api::Function> &fn, std::function<void(td::td_api::object_ptr<td::td_api::Object>)> handler);

private:
    std::map<uint64_t, std::function<void(td::td_api::object_ptr<td::td_api::Object>)>> inflight_requests;
    td::td_api::object_ptr<td::td_api::Update> handle_td_update(td::Client::Response &response);
};

#endif