#ifndef CONNECTIONSTATE_HANDLER_H
#define CONNECTIONSTATE_HANDLER_H

#include "td/telegram/Client.h"
#include <iostream>
#include <assert.h>

class ConnectionStateHandler
{
public:
    ConnectionStateHandler(std::shared_ptr<td::Client> client);
    void handle_connection_state(td::td_api::object_ptr<td::td_api::updateConnectionState> &updateConnectionState);

private:
    std::shared_ptr<td::Client> _client;
};

#endif