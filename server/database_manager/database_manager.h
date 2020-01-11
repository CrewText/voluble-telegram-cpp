#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite_orm.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "User.h"
#include "../logger.h"

using namespace sqlite_orm;

// using DB = internal::storage_t<internal::table_t<User,
//                                                  internal::column_t<User, decltype(User::id), const decltype(User::id) &(User::*)() const, void (User::*)(unsigned int), constraints::autoincrement_t, constraints::primary_key_t<>>,
//                                                  internal::column_t<User, decltype(User::phone_number), const decltype(User::phone_number) &(User::*)() const, void (User::*)(decltype(User::phone_number))>,
//                                                  internal::column_t<User, decltype(User::telegram_id), const decltype(User::telegram_id) &(User::*)() const, void (User::*)(decltype(User::telegram_id))>,
//                                                  internal::column_t<User, decltype(User::first_name), const decltype(User::first_name) &(User::*)() const, void (User::*)(decltype(User::first_name))>,
//                                                  internal::column_t<User, decltype(User::last_name), const decltype(User::last_name) &(User::*)() const, void (User::*)(decltype(User::last_name))>,
//                                                  internal::column_t<User, decltype(User::username), const decltype(User::username) &(User::*)() const, void (User::*)(decltype(User::username))>>>;

using DB = decltype(make_storage("",
                                 make_table("users",
                                            make_column("id", &User::id, autoincrement(), primary_key()),
                                            make_column("phone_number", &User::phone_number),
                                            make_column("telegram_id", &User::telegram_id),
                                            make_column("first_name", &User::first_name),
                                            make_column("last_name", &User::last_name),
                                            make_column("username", &User::username))));

// auto initStorage(std::string db_path)
// {
//     return make_storage("",
//                         make_table("users",
//                                    make_column("id", &User::id, autoincrement(), primary_key()),
//                                    make_column("phone_number", &User::phone_number),
//                                    make_column("telegram_id", &User::telegram_id),
//                                    make_column("first_name", &User::first_name),
//                                    make_column("last_name", &User::last_name),
//                                    make_column("username", &User::username)));
// }

// using DB = decltype(initStorage(""));

// static_assert(std::is_same<DB, decltype(storage)>::value, "");

class DatabaseManager
{
public:
    DatabaseManager(std::string db_location);
    void init_new_user_database();
    bool verify_database();
    std::vector<User> fetch_user_list();
    std::shared_ptr<User> get_user_by_phone_no(std::string phone_no);
    User add_user(std::string phone_number,
                  int telegram_id = 0,
                  std::string first_name = NULL,
                  std::string last_name = NULL,
                  std::string username = NULL);
    User add_user(User user);
    void remove_user(unsigned int user_id);
    void update_user_details(unsigned int user_id,
                             int telegram_id = 0,
                             std::string first_name = nullptr,
                             std::string last_name = nullptr,
                             std::string username = nullptr,
                             std::string phone_number = nullptr);
    void update_user_details(User user);

private:
    std::shared_ptr<spdlog::logger> _logger;
    std::string _db_location;
    // DB db;
    DB db; //= std::move(storage);
    // SQLite::Database *db;

    User create_user_in_db(User user);
};

#endif //DATABASE_MANAGER_H