#include "telegram_client.h"

using namespace std;

vector<td::td_api::object_ptr<td::td_api::updateOption>> options;
shared_ptr<td::Client> _client;
const double WAIT_TIMEOUT = 10.0;
bool is_closed = false;

AuthorizationHandler authHandler = AuthorizationHandler(_client);
OptionHandler optionHandler = OptionHandler(_client);
ConnectionStateHandler connectionStateHandler = ConnectionStateHandler(_client);

TelegramClient::TelegramClient()
{
    td::Log::set_file_path("C:/Users/calmc/Source/Repos/voluble-telegram-cpp/build/log.log");
    td::Log::set_verbosity_level(100);
    td::Log::set_fatal_error_callback(fatal_error_callback);
    td::Client::execute({0, td::td_api::make_object<td::td_api::setLogVerbosityLevel>(5)});
    auto logStreamFile = td::td_api::make_object<td::td_api::logStreamFile>("C:\\Users\\calmc\\Source\\Repos\\voluble-telegram-cpp\\build\\log2.log", 100000000);
    auto setLogStream = td::td_api::make_object<td::td_api::setLogStream>(move(logStreamFile));
    td::Client::execute({10, td::td_api::make_object<td::td_api::setLogStream>(move(logStreamFile))});
    _client = make_shared<td::Client>();
}

void fatal_error_callback(const char *error_message)
{
    cerr << "ERROR: "
         << error_message << endl;
}

void TelegramClient::do_receive_loop()
{
    while (!is_closed)
    {
        auto response = _client->receive(WAIT_TIMEOUT);
        if (response.object == nullptr)
        {
            cout << "Timed out waiting for an update" << endl;
            continue;
        }

        if (response.id == 0)
        {
            // This is an unsolicited update; process it now
            td::td_api::object_ptr<td::td_api::Update> up = handle_td_update(response);
            if (up != nullptr)
            {
                if (up->get_id() == td::td_api::updateOption::ID)
                {
                    options.push_back(td::td_api::move_object_as<td::td_api::updateOption>(up));
                }
            }
        }
        else
        {
            // This is a response to an existing query, hand it over
            auto fn = inflight_requests.find(response.id);
            //inflight_requests.erase(response.id);
            if (fn != inflight_requests.end())
            {
                fn->second(move(response.object));
                inflight_requests.erase(response.id);
            }
        }
    }
}

void TelegramClient::send_query(td::td_api::object_ptr<td::td_api::Function> &fn, std::function<void(td::td_api::object_ptr<td::td_api::Object>)> handler)
{
    uint64_t query_id = rand();
    cout << "Sending query " << query_id << "... ";
    if (handler)
    {
        inflight_requests.emplace(query_id, handler);
    }
    _client->send({query_id, move(fn)});
    cout << " sent." << endl;
}

td::td_api::object_ptr<td::td_api::Update> TelegramClient::handle_td_update(td::Client::Response &response)
{
    cout << "Received update";
    switch (response.object->get_id())
    {
    case td::td_api::updateOption::ID:
    {
        cout << "... it's an OptionValue, handing over to OptionValue handler" << endl;
        auto state = td::td_api::move_object_as<td::td_api::updateOption>(response.object);
        optionHandler.handle_option(state);

        return state;
    }
    case td::td_api::updateAuthorizationState::ID:
    {
        cout << "... it's an AuthState, handing over to AuthState handler" << endl;
        auto authState = td::td_api::move_object_as<td::td_api::updateAuthorizationState>(response.object);
        td::td_api::object_ptr<td::td_api::Function> fn = authHandler.handle_authorization_state(authState);
        if (fn != nullptr)
        {
            send_query(fn, {});
        }

        return authState;
    }
    case td::td_api::updateConnectionState::ID:
    {
        cout << "... it's an updateConnectionState, handing over to ConnectionStateHandler" << endl;
        auto connectionState = td::td_api::move_object_as<td::td_api::updateConnectionState>(response.object);
        connectionStateHandler.handle_connection_state(connectionState);
        return nullptr;
    }
    default:
    {
        auto update = td::move_tl_object_as<td::td_api::Update>(response.object);
        cout << "...unknown. ID: " << update->get_id() << endl;
        return update;
    }
    }
}