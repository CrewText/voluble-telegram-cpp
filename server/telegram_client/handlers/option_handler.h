#ifndef OPTION_HANDLER_H
#define OPTION_HANDLER_H

#include "td/telegram/Client.h"
#include <iostream>
#include <assert.h>

class OptionHandler
{
public:
    OptionHandler(std::shared_ptr<td::Client> client);
    void handle_option(td::td_api::object_ptr<td::td_api::updateOption> &updateOption);

private:
    std::shared_ptr<td::Client> _client;
};

#endif