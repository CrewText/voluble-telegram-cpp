#include "server.h"
using namespace std;

void handle_request()
{
    DatabaseManager *db_manager = new DatabaseManager("C:/Users/calmc/Desktop/voluble-telegram.db");
    TelegramClient *tc = new TelegramClient();
    cout << "Created TC" << endl;
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