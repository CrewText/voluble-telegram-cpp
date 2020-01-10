#include "database_manager.h"
#include "database_exceptions.h"

using namespace std;

const string USER_TABLE_NAME = "contacts";

DatabaseManager::DatabaseManager(string db_location) : db{make_storage(db_location,
                                                                       make_table(USER_TABLE_NAME,
                                                                                  make_column("id", &User::id, autoincrement(), primary_key()),
                                                                                  make_column("phone_number", &User::phone_number),
                                                                                  make_column("telegram_id", &User::telegram_id),
                                                                                  make_column("first_name", &User::first_name),
                                                                                  make_column("last_name", &User::last_name),
                                                                                  make_column("username", &User::username)))} /* : db{initStorage(db_location)}*/
{

    _logger = create_logger("DBManager");
    if (_logger == nullptr)
    {
        cout << "DBManager: No logger found!" << endl;
        exit(1);
    }

    _db_location = db_location;
    _logger->debug("Opened DB at {}", _db_location);
}

void DatabaseManager::init_new_user_database()
{
    db.sync_schema();
}

bool DatabaseManager::verify_database()
{
    _logger->debug("Attempting to verify database at {}", _db_location);
    try
    {
        _logger->debug("Checking for existence of {} table", USER_TABLE_NAME);
        if (!db.table_exists(USER_TABLE_NAME))
        {
            throw UsersTableNotFound();
        }
        else
        {
            _logger->debug("{} table exists", USER_TABLE_NAME);
        }
    }
    catch (system_error e)
    {
        _logger->error("Failed to check if {} table exists: {}", USER_TABLE_NAME, e.what());
    }
    catch (UsersTableNotFound e)
    {
        throw e;
    }
    catch (exception &e)
    {
        _logger->error("Got unknown error when verifying {} table: {}", USER_TABLE_NAME, e.what());
        return false;
    }

    try
    {
        _logger->debug("Attempting to do PRAGMA user version");
        _logger->debug("User version: {}", db.pragma.user_version());
    }
    catch (system_error e)
    {
        throw IntegrityCheckFailed(e.what());
    }
    catch (exception &e)
    {
        _logger->error("Got unknown error when running `vacuum()`: {}", e.what());
        return false;
    }

    return true;
}

vector<User> DatabaseManager::fetch_user_list()
{
    return db.get_all<User>();
}

shared_ptr<User> DatabaseManager::get_user_by_phone_no(string phone_no)
{

    try
    {
        shared_ptr<User> u =
            make_shared<User>(db.get_all<User>(where(c(&User::phone_number) == phone_no)).at(0));
        if (u == nullptr)
        {
            return u;
        }
    }
    catch (system_error e)
    {
        throw UserNotFoundInDB(phone_no);
    }
}

User DatabaseManager::add_user(std::string phone_number,
                               int telegram_id,
                               std::string first_name,
                               std::string last_name,
                               std::string username)
{
    User u = User();
    u.phone_number = phone_number;
    u.telegram_id = make_shared<int>(telegram_id);
    u.first_name = make_shared<string>(first_name);
    u.last_name = make_shared<string>(last_name);
    u.username = make_shared<string>(username);

    return create_user_in_db(move(u));
}

User DatabaseManager::add_user(User user)
{

    return create_user_in_db(move(user));
}

User DatabaseManager::create_user_in_db(User user)
{
    db.begin_transaction();
    try
    {
        user.id = db.insert<User>(user);
        db.commit();
        return user;
    }

    catch (system_error e)
    {
        _logger->error("Error creating user {}: {}", user.id, e.what());
        db.rollback();
    }
}

void DatabaseManager::remove_user(unsigned int user_id)
{
    db.begin_transaction();
    try
    {
        db.remove<User>(user_id);
        db.commit();
    }
    catch (system_error e)
    {
        _logger->error("Error removing user {}: {}", user_id, e.what());
        db.rollback();
    }
}

void DatabaseManager::update_user_details(unsigned int user_id,
                                          int telegram_id,
                                          std::string first_name,
                                          std::string last_name,
                                          std::string username,
                                          std::string phone_number)
{
    User u;
    u.id = user_id;
    u.phone_number = phone_number;
    u.telegram_id = make_shared<int>(telegram_id);
    u.first_name = make_shared<string>(first_name);
    u.last_name = make_shared<string>(last_name);
    u.username = make_shared<string>(username);

    try
    {
        db.begin_transaction();
        db.update<User>(u);
        db.commit();
    }
    catch (system_error e)
    {
        _logger->error("Failed to update user {}: {}", user_id, e.what());
    }
}

void DatabaseManager::update_user_details(User user)
{
    try
    {
        db.begin_transaction();
        db.update<User>(user);
        db.commit();
    }
    catch (system_error e)
    {
        _logger->error("Failed to update user {}: {}", user.id, e.what());
    }
}