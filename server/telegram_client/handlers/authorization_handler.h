#ifndef AUTHORIZATION_HANDLER_H
#define AUTHORIZATION_HANDLER_H

#include "td/telegram/Client.h"
#include "VolubleTelegramClientConfig.h"
#include <iostream>
#include <sstream>
#include <assert.h>
#include "../logger.h"
#include <random>

template <class T>
using TdObjPtr = td::td_api::object_ptr<T>;

class AuthorizationHandler
{
public:
    AuthorizationHandler(std::shared_ptr<td::Client> client, unsigned int client_id = 0);
    virtual ~AuthorizationHandler();
    TdObjPtr<td::td_api::Function> handle_authorization_state(TdObjPtr<td::td_api::updateAuthorizationState> &updateAuthState);

private:
    std::shared_ptr<td::Client> _client;
    std::shared_ptr<spdlog::logger> _logger;
    unsigned int _client_id;
    TdObjPtr<td::td_api::Function> handle_tdlib_parameters();
    TdObjPtr<td::td_api::Function> handle_wait_encryption_key();
    TdObjPtr<td::td_api::Function> handle_wait_phone_number();
    TdObjPtr<td::td_api::Function> handle_wait_code();
};
#endif