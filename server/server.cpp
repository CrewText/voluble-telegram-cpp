#include "server.h"
using namespace std;

void handle_request()
{
    std::shared_ptr<spdlog::logger> _logger = spdlog::get("console");
    _logger->debug("Creating DB Manager");
    DatabaseManager *db_manager = new DatabaseManager("C:\\Users\\calmc\\Desktop\\voluble-telegram.db");
    _logger->debug("Creating Telegram Client");
    TelegramClient *tc = new TelegramClient();

    if (!db_manager->verify_database())
    {
        db_manager->init_new_user_database();
    };

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

        tc->do_receive_loop();
    }
    catch (exception const &e)
    {
        cout << e.what() << endl;
    }
}

void handle_get(web::http::http_request request)
{
    cout << "Doing GET" << endl;
}