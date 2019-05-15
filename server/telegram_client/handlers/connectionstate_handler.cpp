#include "connectionstate_handler.h"

using namespace std;

ConnectionStateHandler::ConnectionStateHandler(std::shared_ptr<td::Client> client)
{
    _client = client;
    _logger = spdlog::get("console");
    if (_logger == nullptr)
    {
        cout << "ConnStateHandler: No logger found!" << endl;
    }
}

ConnectionStateHandler::~ConnectionStateHandler()
{
    _client = nullptr;
}

void ConnectionStateHandler::handle_connection_state(td::td_api::object_ptr<td::td_api::updateConnectionState> &updateConnectionState)
{
    // cout << "Connection state: ";
    switch (updateConnectionState->state_->get_id())
    {
    case td::td_api::connectionStateConnecting::ID:
    {
        _logger->info("Connection state: Connecting");
        // cout << "Connecting" << endl;
        break;
    }
    case td::td_api::connectionStateConnectingToProxy::ID:
    {
        _logger->info("Connection state: Connecting to proxy");
        // cout << "Connecting to proxy" << endl;
        break;
    }
    case td::td_api::connectionStateWaitingForNetwork::ID:
    {
        _logger->info("Connection state: Waiting for network");
        // cout << "Waiting for network" << endl;
        break;
    }
    case td::td_api::connectionStateUpdating::ID:
    {
        _logger->info("Connection state: Updating");
        // cout << "Updating" << endl;
        break;
    }
    case td::td_api::connectionStateReady::ID:
    {
        _logger->info("Connection state: Ready");
        // cout << "Ready" << endl;
        break;
    }
    default:
    {
        _logger->warn("ConnectionStateHandler: unknown update ID: {}", updateConnectionState->get_id());
        // cout << "ConnectionStateHandler: unknown update ID: " << updateConnectionState->get_id() << endl;
        assert(false);
    }
    }
}