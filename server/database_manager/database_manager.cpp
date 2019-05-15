#include "database_manager.h"

using namespace std;

DatabaseManager::DatabaseManager(string db_location) : db(db_location, SQLite::OPEN_CREATE)
{
    _db_location = db_location;
    _logger = spdlog::get("console");
    if (_logger == nullptr)
    {
        cout << "DBManager: No logger found!" << endl;
        exit(1);
    }
}

void DatabaseManager::init_new_user_database()
{
    string query = "CREATE TABLE \"users\" (\
	\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,\
	\"phone_number\" TEXT NOT NULL UNIQUE);";
    SQLite::Statement *stmt = new SQLite::Statement(db, query);
    try
    {
        int result = stmt->exec();
    }
    catch (SQLite::Exception e)
    {
        _logger->error(e.getErrorStr());
    }
}