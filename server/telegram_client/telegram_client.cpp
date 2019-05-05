#include "telegram_client.h"

using namespace std;

vector<td::td_api::object_ptr<td::td_api::updateOption>> options;
shared_ptr<td::Client> _client;
const double WAIT_TIMEOUT = 10.0;
bool is_closed = false;

AuthorizationHandler authHandler = AuthorizationHandler(_client);

TelegramClient::TelegramClient()
{
    _client = make_shared<td::Client>();
    _client->execute({0, td::td_api::make_object<td::td_api::setLogVerbosityLevel>(10)});
    auto logStreamFile = td::td_api::make_object<td::td_api::logStreamFile>("C:/Users/calmc/Source/Repos/voluble-telegram-cpp/build/log.log", 100000000);
    auto setLogStream = td::td_api::make_object<td::td_api::setLogStream>(move(logStreamFile));
    _client->execute({10, td::td_api::make_object<td::td_api::setLogStream>(logStreamFile)});
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
            if (up->get_id() == td::td_api::updateOption::ID)
            {
                options.push_back(td::move_tl_object_as<td::td_api::updateOption>(up));
            }
        }
    }
}

td::td_api::object_ptr<td::td_api::Update> TelegramClient::handle_td_update(td::Client::Response &response)
{
    //   td::td_api::object_ptr<td::td_api::Object> update = response.object;
    cout << "Got an update of type " << response.object->get_id() << endl;
    switch (response.object->get_id())
    {
    case td::td_api::updateOption::ID:
    {
        cout << "it's an update option!" << endl;
        auto state = td::move_tl_object_as<td::td_api::updateOption>(response.object);
        cout << state->name_ << ": " << &state->value_ << endl;
        return state;
    }
    case td::td_api::updateAuthorizationState::ID:
    {
        cout << "Got an AuthState, handing over to AuthState handler" << endl;
        auto authState = td::move_tl_object_as<td::td_api::updateAuthorizationState>(response.object);
        authHandler.handle_authorization_state(authState);
        break;
    }
    default:
    {
        return td::move_tl_object_as<td::td_api::Update>(response.object);
    }
    }
}