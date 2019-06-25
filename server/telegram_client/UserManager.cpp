#include "UserManager.h"

using namespace std;

UserManager::UserManager(TC *const tc)
{
    _tc = tc;
}

UserManager::~UserManager()
{
    delete _tc;
}

vector<int32_t> UserManager::getAllTdUserIDs()
{
}

User UserManager::getUserByTdId(int32_t telegram_id)
{
    User u;
    td::td_api::object_ptr<td::td_api::Function> fn = td::td_api::make_object<td::td_api::getUser>(telegram_id);

    promise<td::td_api::object_ptr<td::td_api::Object>> prom = _tc->send_query(fn);
    prom.get_future().wait();

    td::td_api::object_ptr<td::td_api::Object> user_tdobj = prom.get_future().get();
    td::td_api::object_ptr<td::td_api::user> user_tduser = td::td_api::move_object_as<td::td_api::user>(user_tdobj);

    u.first_name = make_shared<string>(user_tduser->first_name_);
    u.last_name = make_shared<string>(user_tduser->last_name_);
    u.telegram_id = make_shared<int>(user_tduser->id_);
    u.phone_number = user_tduser->phone_number_;
    u.username = make_shared<string>(user_tduser->username_);

    return u;
}
