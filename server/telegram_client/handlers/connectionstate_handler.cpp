#include "connectionstate_handler.h"

using namespace std;

ConnectionStateHandler::ConnectionStateHandler(std::shared_ptr<td::Client> client)
{
    _client = client;
    _logger = create_logger("ConnStateHandler");
    if (_logger == nullptr)
    {
        cout << "ConnStateHandler: No logger found!" << endl;
    }
}

ConnectionStateHandler::~ConnectionStateHandler()
{
    _client = nullptr;
}

td::td_api::object_ptr<td::td_api::Function> ConnectionStateHandler::handle_connection_state(td::td_api::object_ptr<td::td_api::updateConnectionState> &updateConnectionState)
{
    // cout << "Connection state: ";
    switch (updateConnectionState->state_->get_id())
    {
    case td::td_api::connectionStateConnecting::ID:
    {
        _logger->info("Connection state: Connecting");
        // cout << "Connecting" << endl;
        return nullptr;
    }
    case td::td_api::connectionStateConnectingToProxy::ID:
    {
        _logger->info("Connection state: Connecting to proxy");
        // cout << "Connecting to proxy" << endl;
        return nullptr;
    }
    case td::td_api::connectionStateWaitingForNetwork::ID:
    {
        _logger->info("Connection state: Waiting for network");
        // cout << "Waiting for network" << endl;
        return nullptr;
    }
    case td::td_api::connectionStateUpdating::ID:
    {
        _logger->info("Connection state: Updating");
        // cout << "Updating" << endl;
        return nullptr;
    }
    case td::td_api::connectionStateReady::ID:
    {
        _logger->info("Connection state: Ready");
        // cout << "Ready" << endl;
        // _logger->debug("Getting contacts");
        // return td::td_api::make_object<td::td_api::searchContacts>("", 2147483647);
        // return td::td_api::make_object<td::td_api::getChats>(2 ^ 63 - 1, 0, 10000);
        // return td::td_api::make_object<td::td_api::getContacts>();
        return nullptr;
    }
    default:
    {
        _logger->warn("Unknown update ID in updateConnectionState: {}", updateConnectionState->get_id());
        // cout << "ConnectionStateHandler: unknown update ID: " << updateConnectionState->get_id() << endl;
        assert(false);
    }
    }
}