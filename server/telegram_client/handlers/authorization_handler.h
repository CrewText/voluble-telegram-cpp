#ifndef AUTHORIZATION_HANDLER_H
#define AUTHORIZATION_HANDLER_H

#include "td/telegram/Client.h"
#include "VolubleTelegramClientConfig.h"
#include <iostream>
#include <sstream>

class AuthorizationHandler
{
public:
    AuthorizationHandler(std::shared_ptr<td::Client> client);

    void handle_authorization_state(td::td_api::object_ptr<td::td_api::updateAuthorizationState> &updateAuthState);

private:
    std::shared_ptr<td::Client> _client;
    void handle_tdlib_parameters();
    void handle_wait_encryption_key();
};
#endif