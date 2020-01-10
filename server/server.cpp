#include "server.h"
using namespace std;

Server::Server()
{
    _logger = create_logger("Server");
    if (_logger == nullptr)
    {
        cout << "Server: No logger found!" << endl;
        exit(1);
    }

    _logger->debug("Creating DB Manager");
    if (getenv("TELEGRAM_DB_LOCATION") == NULL)
    {
        _logger->critical("Env var 'TELEGRAM_DB_LOCATION' not found, no database can be used! Exiting...");
        exit(EXIT_FAILURE);
    }

    db_manager = new DatabaseManager(getenv("TELEGRAM_DB_LOCATION"));

    try
    {
        _logger->debug("Verifying database");
        db_manager->verify_database();
        _logger->debug("Database verified");
    }
    catch (IntegrityCheckFailed &e)
    {
        _logger->warn(e.what());
    }
    catch (const UsersTableNotFound &e)
    {
        _logger->info("Users table not found, creating...");
        db_manager->init_new_user_database();
    }
    catch (exception &e)
    {
        _logger->error("Caught error when verifying DB: {}", e.what());
    }

    vector<User> user_list;

    try
    {
        _logger->debug("Fetching user list");
        user_list = db_manager->fetch_user_list();
        _logger->debug("Found {} users", user_list.size());
    }
    catch (exception &e)
    {
        _logger->error("Caught error when fetching user list: {}", e.what());
    }
    try
    {
        for (int i = 0; i < user_list.size(); i++)
        {
            _logger->debug("Creating Telegram Client for user ID {}, phone no: {}", user_list.at(i).id, user_list.at(i).phone_number);
            TC *tc = new TC(user_list.at(i).id);
            _logger->debug("Client created");

            thread *t = new thread(&TC::do_receive_loop, tc);
            client_threads.emplace_back(t);
        }
    }
    catch (exception &e)
    {
        _logger->error("Caught error creating TCs: {}", e.what());
    }
}

Server::~Server()
{
    for (int i = 0; i < client_threads.size(); i++)
    {
        client_threads.at(i)->join();
    }
}

void Server::handle_request()
{

    try
    {
        // string stringhost = "http://localhost";
        // const utility::string_t uri = utility::conversions::to_string_t(stringhost);

        // web::http::experimental::listener::http_listener listener(uri);

        // listener.support(web::http::methods::GET, handle_get);
        // listener.open()
        //     .then([=]() {
        //         cout << "Starting to listen..." << endl;
        //     })
        //     .wait();
    }
    catch (exception const &e)
    {
        cout << e.what() << endl;
    }
}

// void Server::handle_get(web::http::http_request request)
// {
//     cout << "Doing GET" << endl;
// }