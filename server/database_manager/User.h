#ifndef USER_H
#define USER_H

#include <string>
#include <memory>

// class User
// {

// public:
//     int id;
//     std::string phone_number;
//     std::unique_ptr<uint32_t> telegram_id;
//     std::unique_ptr<std::string> first_name;
//     std::unique_ptr<std::string> last_name;
//     std::unique_ptr<std::string> username;
//     User();
//     User(User &u);
//     User(unsigned int user_id,
//          std::string user_phone_number,
//          uint32_t user_telegram_id = NULL,
//          std::string user_first_name = NULL,
//          std::string user_last_name = NULL,
//          std::string user_username = NULL);
// };

struct User
{
    unsigned int id;
    std::string phone_number;
    // std::unique_ptr<int> telegram_id;
    // std::unique_ptr<std::string> first_name;
    // std::unique_ptr<std::string> last_name;
    // std::unique_ptr<std::string> username;
    std::shared_ptr<int> telegram_id;
    std::shared_ptr<std::string> first_name;
    std::shared_ptr<std::string> last_name;
    std::shared_ptr<std::string> username;
};

#endif //USER_H