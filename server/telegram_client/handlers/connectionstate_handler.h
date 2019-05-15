#ifndef CONNECTIONSTATE_HANDLER_H
#define CONNECTIONSTATE_HANDLER_H

#include "td/telegram/Client.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <assert.h>

class ConnectionStateHandler
{
public:
    ConnectionStateHandler(std::shared_ptr<td::Client> client);
    virtual ~ConnectionStateHandler();
    void handle_connection_state(td::td_api::object_ptr<td::td_api::updateConnectionState> &updateConnectionState);

private:
    std::shared_ptr<td::Client> _client;
    std::shared_ptr<spdlog::logger> _logger;
};

#endif