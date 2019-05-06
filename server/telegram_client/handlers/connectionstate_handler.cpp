#include "connectionstate_handler.h"

using namespace std;

ConnectionStateHandler::ConnectionStateHandler(std::shared_ptr<td::Client> client)
{
    _client = client;
}

void ConnectionStateHandler::handle_connection_state(td::td_api::object_ptr<td::td_api::updateConnectionState> &updateConnectionState)
{
    cout << "Connection state: ";
    switch (updateConnectionState->state_->get_id())
    {
    case td::td_api::connectionStateConnecting::ID:
    {
        cout << "Connecting" << endl;
        break;
    }
    case td::td_api::connectionStateConnectingToProxy::ID:
    {
        cout << "Connecting to proxy" << endl;
        break;
    }
    case td::td_api::connectionStateWaitingForNetwork::ID:
    {
        cout << "Waiting for network" << endl;
        break;
    }
    case td::td_api::connectionStateUpdating::ID:
    {
        cout << "Updating" << endl;
        break;
    }
    case td::td_api::connectionStateReady::ID:
    {
        cout << "Ready" << endl;
        break;
    }
    default:
    {
        cout << "ConnectionStateHandler: unknown update ID: " << updateConnectionState->get_id() << endl;
        assert(false);
    }
    }
}