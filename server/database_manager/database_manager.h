#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "spdlog/spdlog.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <iostream>

class DatabaseManager
{
public:
    DatabaseManager(std::string db_location);
    void init_new_user_database();
    bool verify_database();

private:
    std::shared_ptr<spdlog::logger> _logger;
    std::string _db_location;
    SQLite::Database db;
};

#endif //DATABASE_MANAGER_H