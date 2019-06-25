#ifndef VTC_USERMANAGER_H
#define VTC_USERMANAGER_H

#include "../database_manager/User.h"
#include "../../logger.h"
#include "telegram_client.h"
#include <vector>
#include <future>
#include <string>
#include "td/telegram/Client.h"

class UserManager
{
public:
    UserManager();
    UserManager(TC *const tc);
    virtual ~UserManager();

    std::vector<int32_t> getAllTdUserIDs();
    User getUserByTdId(int32_t telegram_id);

private:
    TC *_tc;
};

#endif