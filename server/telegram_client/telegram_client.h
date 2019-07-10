#ifndef VTC_TELEGRAM_CLIENT_H
#define VTC_TELEGRAM_CLIENT_H
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <random>
#include <functional>
#include <sstream>
#include <future>
#include <thread>

#include "td/telegram/Client.h"
#include "td/telegram/Log.h"
#include "handlers/authorization_handler.h"
#include "handlers/option_handler.h"
#include "handlers/connectionstate_handler.h"
#include "../database_manager/database_manager.h"
#include "../logger.h"

using TdObjectPtr = td::td_api::object_ptr<td::td_api::Object>;
using RequestPromise_t = std::shared_ptr<std::promise<std::shared_ptr<TdObjectPtr>>>;

void fatal_error_callback(const char *error_message);
class TC
{
public:
    TC(unsigned int ID = 0);
    virtual ~TC();
    void do_receive_loop();
    // void send_query(td::td_api::object_ptr<td::td_api::Function> &fn, std::function<void(TdObjectPtr)> handler);
    RequestPromise_t send_query(td::td_api::object_ptr<td::td_api::Function> &fn);
    bool error_check(std::shared_ptr<TdObjectPtr> obj_ptr);

private:
    std::shared_ptr<spdlog::logger> _logger;

    bool is_closed = false;

    std::shared_ptr<td::Client> _client;
    std::vector<td::td_api::object_ptr<td::td_api::OptionValue>> options;
    //std::map<uint64_t, std::function<void(TdObjectPtr)>> inflight_requests;
    std::map<uint64_t, RequestPromise_t> inflight_requests;
    AuthorizationHandler *authHandler;
    OptionHandler *optionHandler;
    ConnectionStateHandler *connectionStateHandler;
    DatabaseManager *dbManager;
    // UserManager *userManager;

    void handle_td_update(std::shared_ptr<TdObjectPtr> update_obj);
};

#endif