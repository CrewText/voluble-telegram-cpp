#include "User.h"

using namespace std;

// User::User(unsigned int user_id,
//            string user_phone_number,
//            uint32_t user_telegram_id,
//            string user_first_name,
//            string user_last_name,
//            string user_username)
// {
//     id = user_id;
//     phone_number = user_phone_number;
//     telegram_id = user_telegram_id ? make_unique<uint32_t>(user_telegram_id) : nullptr;
//     first_name = user_first_name.length() ? make_unique<string>(user_first_name) : nullptr;
//     last_name = user_last_name.length() ? make_unique<string>(user_last_name) : nullptr;
//     username = user_username.length() ? make_unique<string>(user_username) : nullptr;
// }

// User::User()
// {
//     id = NULL;
//     phone_number = nullptr;
//     telegram_id = nullptr;
//     first_name = nullptr;
//     last_name = nullptr;
//     username = nullptr;
// }

// User::User(User &u)
// {
//     id = move(u.id);
//     phone_number = move(u.phone_number);
//     telegram_id = move(u.telegram_id);
//     first_name = move(u.first_name);
//     last_name = move(u.last_name);
//     username = move(u.username);
// }