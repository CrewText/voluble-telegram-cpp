#include "telegram_client.h"

using namespace std;

const double TELEGRAM_WAIT_TIMEOUT = 10.0;

TelegramClient::TelegramClient()
{
    _logger = spdlog::get("console");
    if (_logger == nullptr)
    {
        cout << "TC: No logger found!" << endl;
        exit(1);
    }

    td::Log::set_file_path("C:/Users/calmc/Source/Repos/voluble-telegram-cpp/build/log.log");
    td::Log::set_verbosity_level(100);
    td::Log::set_fatal_error_callback(fatal_error_callback);
    td::Client::execute({0, td::td_api::make_object<td::td_api::setLogVerbosityLevel>(5)});
    auto logStreamFile = td::td_api::make_object<td::td_api::logStreamFile>("C:\\Users\\calmc\\Source\\Repos\\voluble-telegram-cpp\\build\\log2.log", 100000000);
    auto setLogStream = td::td_api::make_object<td::td_api::setLogStream>(move(logStreamFile));
    td::Client::execute({10, td::td_api::make_object<td::td_api::setLogStream>(move(logStreamFile))});
    _client = make_shared<td::Client>();

    authHandler = new AuthorizationHandler(_client);
    optionHandler = new OptionHandler(_client);
    connectionStateHandler = new ConnectionStateHandler(_client);
}

TelegramClient::~TelegramClient()
{
    _logger->info("Destroying TelegramClient");
    td::td_api::object_ptr<td::td_api::destroy> fn = td::td_api::make_object<td::td_api::destroy>();
    _client->execute({99999, td::td_api::make_object<td::td_api::destroy>()});
}

void fatal_error_callback(const char *error_message)
{
    cerr
        << "ERROR: "
        << error_message << endl;
}

void TelegramClient::do_receive_loop()
{
    while (!is_closed)
    {
        auto response = _client->receive(TELEGRAM_WAIT_TIMEOUT);
        if (response.object == nullptr)
        {
            _logger->info("Timed out waiting for an update");
            continue;
        }

        if (response.id == 0)
        {
            // This is an unsolicited update; process it now
            handle_td_update(response);
        }
        else
        {
            // This is a response to an existing query, hand it over
            auto fn = inflight_requests.find(response.id);
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
    this->_logger->debug("Sending query #{}", query_id);
    if (handler)
    {
        inflight_requests.emplace(query_id, handler);
    }
    _client->send({query_id, move(fn)});
    this->_logger->debug("Query #{} sent", query_id);
}

void TelegramClient::handle_td_update(td::Client::Response &response)
{
    switch (response.object->get_id())
    {
    case td::td_api::updateOption::ID:
    {
        _logger->debug("Received an OptionValue, handing over to OptionValue handler");
        auto state = td::td_api::move_object_as<td::td_api::updateOption>(response.object);
        optionHandler->handle_option(state);
        options.push_back(move(state->value_));
        break;
    }

    case td::td_api::updateAuthorizationState::ID:
    {
        _logger->debug("Received an AuthState, handing over to AuthState handler");
        auto authState = td::td_api::move_object_as<td::td_api::updateAuthorizationState>(response.object);
        td::td_api::object_ptr<td::td_api::Function> fn = authHandler->handle_authorization_state(authState);
        if (fn != nullptr)
        {
            send_query(fn, {});
        }
        break;
    }

    case td::td_api::updateConnectionState::ID:
    {
        _logger->debug("Received an updateConnectionState, handing over to ConnectionStateHandler");
        auto connectionState = td::td_api::move_object_as<td::td_api::updateConnectionState>(response.object);
        connectionStateHandler->handle_connection_state(connectionState);
        break;
    }

    case td::td_api::updateScopeNotificationSettings::ID:
    {
        _logger->debug("Received an updateScopeNotificationSettings, not terribly important");
        break;
    }

    case td::td_api::updateHavePendingNotifications::ID:
    {
        _logger->debug("Received an updateHavePendingNotifications: ");
        auto updateHavePendingNotifications = td::td_api::move_object_as<td::td_api::updateHavePendingNotifications>(response.object);
        if (updateHavePendingNotifications->have_delayed_notifications_)
        {
            _logger->debug("Have delayed notifications; ");
        }
        if (updateHavePendingNotifications->have_unreceived_notifications_)
        {
            _logger->debug("Have unreceived notifications");
        }
        break;
    }

    case td::td_api::updateTermsOfService::ID:
    {
        _logger->warn("Received an updateTermsOfService, this should be implemented in the future");
        auto updateToS = td::td_api::move_object_as<td::td_api::updateTermsOfService>(response.object);
        cout
            << "Min user age: " << updateToS->terms_of_service_->min_user_age_ << endl;
        break;
    }

    case td::td_api::updateNewMessage::ID:
    {
        _logger->warn("Received an updateNewMessage, implement me!");
        auto updateNewMessage = td::td_api::move_object_as<td::td_api::updateNewMessage>(response.object);
        //TODO: BUILD HANDLER FOR UPDATENEWMESSAGE
        break;
    }

    case td::td_api::updateNewChat::ID:
    {
        _logger->warn("Received an updateNewChat, implement me!");
        auto updateNewChat = td::td_api::move_object_as<td::td_api::updateNewChat>(response.object);
        //TODO: BUILD HANDLER FOR UPDATENEWCHAT
        break;
    }

    default:
    {
        auto update = td::move_tl_object_as<td::td_api::Update>(response.object);
        _logger->info("Received unknown update. ID: {}", update->get_id());
    }
    }
}