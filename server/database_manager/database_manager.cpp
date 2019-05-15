#include "database_manager.h"

using namespace std;

#ifdef SQLITECPP_ENABLE_ASSERT_HANDLER
{
    /// definition of the assertion handler enabled when SQLITECPP_ENABLE_ASSERT_HANDLER is defined in the project (CMakeList.txt)
    void SQLite::assertion_failed(const char *apFile, const long apLine, const char *apFunc, const char *apExpr, const char *apMsg)
    {
        // Print a message to the standard error output stream, and abort the program.
        cerr << apFile << ":" << apLine << ":"
             << " error: assertion failed (" << apExpr << ") in " << apFunc << "() with message \"" << apMsg << "\"\n";
        abort();
    }
}
#endif

DatabaseManager::DatabaseManager(string db_location)
{
    _logger = spdlog::get("console");
    if (_logger == nullptr)
    {
        cout << "DBManager: No logger found!" << endl;
        exit(1);
    }

    _db_location = db_location;
    try
    {
        _logger->info("Opening database at: {}", db_location);
        db = new SQLite::Database(db_location, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    }
    catch (SQLite::Exception e)
    {
        _logger->error(e.getErrorStr());
    }
}

void DatabaseManager::init_new_user_database()
{
    string query = "CREATE TABLE \"users\" (\
	\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,\
	\"phone_number\" TEXT NOT NULL UNIQUE);";
    SQLite::Statement stmt(*db, query);
    SQLite::Transaction trn(*db);
    try
    {
        int result = stmt.exec();
        trn.commit();
    }
    catch (SQLite::Exception e)
    {
        _logger->error(e.getErrorStr());
    }
}

bool DatabaseManager::verify_database()
{
    bool table_exists = db->tableExists("users");
    if (!table_exists)
    {
        _logger->warn("DBManager: Users table does not exist in local DB");
        return false;
    }

    SQLite::Statement stmt(*db, "PRAGMA integrity_check;");
    bool result = stmt.executeStep();
    if (!result)
    {
        _logger->warn("DBManager: Integrity check query failed to return a row");
        return false;
    }

    SQLite::Column col = stmt.getColumn(0);
    string data = col.getString();
    if (data == "ok")
    {
        return true;
    }
    else
    {
        _logger->warn("Integrity check failed; data returned: {}", data);
        return false;
    }
}