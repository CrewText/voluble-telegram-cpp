#ifndef TELEGRAM_CLIENT_H
#define TELEGRAM_CLIENT_H
#include <vector>
#include <string>
#include <iostream>
#include "td/telegram/Client.h"
#include "td/telegram/Log.h"
#include "handlers/authorization_handler.h"

class TelegramClient
{
public:
    TelegramClient();
    void do_receive_loop();

private:
    td::td_api::object_ptr<td::td_api::Update> handle_td_update(td::Client::Response &response);
};

#endif