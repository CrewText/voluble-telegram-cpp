#include "telegram_client.h"

using namespace std;

const double TELEGRAM_WAIT_TIMEOUT = 10.0;

TC::TC(unsigned int ID)
{
    stringstream logger_name;
    logger_name << "TC_" << ID;
    _logger = create_logger(logger_name.str());
    if (_logger == nullptr)
    {
        cout << "TC: No logger found!" << endl;
        exit(1);
    }

    if (getenv("TD_LOG_PATH") == NULL)

    {
        _logger->warn("Env var 'TD_LOG_PATH' not set, logging to stdout...");
    }
    else
    {
        td::Log::set_file_path(getenv("TD_LOG_PATH"));
    }
    td::Log::set_verbosity_level(5);
    td::Log::set_fatal_error_callback(fatal_error_callback);
    _client = make_shared<td::Client>();

    stringstream db_location;
    db_location << getenv("TELEGRAM_DB_LOCATION") << "_contacts_" << ID;

    authHandler = new AuthorizationHandler(_client, ID);
    optionHandler = new OptionHandler(_client);
    connectionStateHandler = new ConnectionStateHandler(_client);
    dbManager = new DatabaseManager(db_location.str());
    // userManager = new UserManager(this);
}

TC::~TC()
{
    _logger->info("Destroying TC");
    td::td_api::object_ptr<td::td_api::destroy> fn = td::td_api::make_object<td::td_api::destroy>();
    _client->execute({99999, td::td_api::make_object<td::td_api::destroy>()});
    delete authHandler;
    delete optionHandler;
    delete connectionStateHandler;
    delete dbManager;
    // delete userManager;
}

void fatal_error_callback(const char *error_message)
{
    cerr
        << "ERROR: "
        << error_message << endl;
}

void TC::do_receive_loop()
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
            _logger->debug("Handling unsolicited update");
            // This is an unsolicited update; process it now
            _logger->trace("Update pointer as nullptr pre-thread: {}", response.object == nullptr);
            auto a = async(launch::async, [this, &response] {
                _logger->trace("Update pointer as nullptr in-thread: {}", response.object == nullptr);
                this->handle_td_update(move(response.object));
            });

            _logger->trace("Created async to handle update");
        }
        else
        {
            // This is a response to an existing query, hand it over

            RequestPromise_t request = inflight_requests.at(response.id);

            request->set_value(move(response.object));
            // inflight_requests.erase(response.id);
        }
    }
}

shared_ptr<promise<td::td_api::object_ptr<td::td_api::Object>>> TC::send_query(td::td_api::object_ptr<td::td_api::Function> &fn)
{
    uint64_t query_id = rand();
    this->_logger->debug("Sending query #{}", query_id);

    promise<td::td_api::object_ptr<td::td_api::Object>> prom;
    shared_ptr<promise<td::td_api::object_ptr<td::td_api::Object>>> prom_ptr = make_shared<promise<td::td_api::object_ptr<td::td_api::Object>>>(move(prom));
    inflight_requests.emplace(query_id, prom_ptr);

    _client->send({query_id, move(fn)});
    this->_logger->debug("Query #{} sent", query_id);
    return prom_ptr;
}

void TC::handle_td_update(td::td_api::object_ptr<td::td_api::Object> update_obj)
{
    switch (update_obj->get_id())
    {
    case td::td_api::error::ID:
    {
        auto err = td::td_api::move_object_as<td::td_api::error>(update_obj);
        _logger->error("Received an error: {}", err->message_);
        break;
    }
    case td::td_api::updateOption::ID:
    {
        _logger->debug("Received an OptionValue, handing over to OptionValue handler");
        td::td_api::object_ptr<td::td_api::updateOption> updateOption = td::td_api::move_object_as<td::td_api::updateOption>(update_obj);
        _logger->trace("pre-thread updateOption == nullptr?: {}", updateOption == nullptr);
        _logger->trace("Converted TD object to updateOption, passing to OptionHandler");
        auto a = async(launch::async, [this, &updateOption] {
            _logger->trace("Opened new async to handle option");
            _logger->trace("updateOption == nullptr?: {}", updateOption == nullptr);
            optionHandler->handle_option(updateOption);

            _logger->trace("Option handled, adding option to list");
            options.push_back(move(updateOption->value_));
        });
        //a.

        //optionHandler->handle_option(state);
        // _logger->trace("Adding option to list");

        break;
    }

    case td::td_api::updateAuthorizationState::ID:
    {
        _logger->debug("Received an AuthState, handing over to AuthState handler");
        auto authState = td::td_api::move_object_as<td::td_api::updateAuthorizationState>(update_obj);

        auto a = async(launch::async, [this, &authState] {
            td::td_api::object_ptr<td::td_api::Function> fn = authHandler->handle_authorization_state(authState);
            if (fn != nullptr)
            {
                shared_ptr<promise<td::td_api::object_ptr<td::td_api::Object>>> prom = send_query(fn);
                // prom->get_future().wait();
                // error_check(prom->get_future().get());
            }
        });

        break;
    }

    case td::td_api::updateConnectionState::ID:
    {
        _logger->debug("Received an updateConnectionState, handing over to ConnectionStateHandler");
        auto connectionState = td::td_api::move_object_as<td::td_api::updateConnectionState>(update_obj);
        td::td_api::object_ptr<td::td_api::Function> fn = connectionStateHandler->handle_connection_state(connectionState);
        if (fn != nullptr)
        {
            auto prom_fn = send_query(fn);
            _logger->trace("Sent getContacts, waiting for response");

            packaged_task<td::td_api::object_ptr<td::td_api::Object>()> task([=] {
                prom_fn->get_future().wait();
                return prom_fn->get_future().get();
            });
            task.make_ready_at_thread_exit();
            task();

            _logger->trace("Got getContacts response");

            auto obj = prom_fn->get_future().get();
            switch (obj->get_id())
            {
            // Sent by ConnectionStateReady - get contacts when connected
            case td::td_api::users::ID:
                td::td_api::object_ptr<td::td_api::users> users = td::td_api::move_object_as<td::td_api::users>(obj);
                _logger->info("Found {} users; adding to Contacts DB", users->total_count_);

                for (unsigned int i = 0; i < users->total_count_; i++)
                {
                    User u;

                    td::td_api::object_ptr<td::td_api::Function> fn = td::td_api::make_object<td::td_api::getUser>(users->user_ids_.at(i));

                    shared_ptr<promise<td::td_api::object_ptr<td::td_api::Object>>> prom_getuser = send_query(fn);
                    prom_getuser->get_future().wait();

                    td::td_api::object_ptr<td::td_api::Object> user_tdobj = prom_getuser->get_future().get();
                    td::td_api::object_ptr<td::td_api::user> user_tduser = td::td_api::move_object_as<td::td_api::user>(user_tdobj);

                    u.first_name = make_shared<string>(user_tduser->first_name_);
                    u.last_name = make_shared<string>(user_tduser->last_name_);
                    u.telegram_id = make_shared<int>(user_tduser->id_);
                    u.phone_number = user_tduser->phone_number_;
                    u.username = make_shared<string>(user_tduser->username_);

                    // User contact = userManager->getUserByTdId(users->user_ids_.at(i));
                    _logger->debug("Got user {}: {} {}", u.id, u.first_name ? *u.first_name : "", u.last_name ? *u.last_name : "");
                }

                obj, users = nullptr;
            }
        }
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
        break;
    }

    case td::td_api::updateTermsOfService::ID:
    {
        _logger->warn("Received an updateTermsOfService, this should be implemented in the future");
        auto updateToS = td::td_api::move_object_as<td::td_api::updateTermsOfService>(update_obj);
        cout
            << "Min user age: " << updateToS->terms_of_service_->min_user_age_ << endl;
        break;
    }

    case td::td_api::updateNewMessage::ID:
    {
        _logger->warn("Received an updateNewMessage, implement me!");
        auto updateNewMessage = td::td_api::move_object_as<td::td_api::updateNewMessage>(update_obj);
        //TODO: BUILD HANDLER FOR UPDATENEWMESSAGE
        break;
    }

    case td::td_api::updateNewChat::ID:
    {
        _logger->warn("Received an updateNewChat, implement me!");
        auto updateNewChat = td::td_api::move_object_as<td::td_api::updateNewChat>(update_obj);
        //TODO: BUILD HANDLER FOR UPDATENEWCHAT
        break;
    }

    case td::td_api::updateUser::ID:
    {
        _logger->warn("Received an updateUser, implementation still WIP");
        auto updateUser = td::td_api::move_object_as<td::td_api::updateUser>(update_obj);
        auto user = move(updateUser->user_);
        stringstream user_phone_no;
        if (user->phone_number_.find("+") == string::npos)
        {
            user_phone_no << "+";
        }
        user_phone_no << user->phone_number_;
        shared_ptr<User> user_in_db = dbManager->get_user_by_phone_no(user_phone_no.str());
        if (user_in_db != nullptr)
        {
            _logger->info("Updating user {} details", user_in_db->id);
            User u = *user_in_db;
            u.id = user_in_db->id;
            u.telegram_id = make_shared<int>(user->id_);
            u.first_name = make_shared<string>(user->first_name_);
            u.last_name = make_shared<string>(user->last_name_);
            u.username = make_shared<string>(user->username_);
            // dbManager->update_user_details(user_in_db->id, user->id_, user->first_name_, user->last_name_, user->username_);
            dbManager->update_user_details(u);
        }
        else
        {
            _logger->warn("User with phone no. {} does not exist in DB; is this correct? User details:", user_phone_no.str());
        }
        break;
    }

    default:
    {
        auto update = td::move_tl_object_as<td::td_api::Update>(update_obj);
        _logger->info("Received unknown update. ID: {}", update->get_id());
        break;
    }
    }
}

bool TC::error_check(td::td_api::object_ptr<td::td_api::Object> obj)
{
    if (obj->get_id() == td::td_api::error::ID)
    {
        try
        {
            auto err = td::td_api::move_object_as<td::td_api::error>(obj);
            if (err->code_ == 406)
            {
                _logger->error("{}", err->message_);
            }
            else
            {
                _logger->error("Received an error: {}", err->message_);
            }
            // td::td_api::object_ptr<td::td_api::Function> fn = td::td_api::make_object<td::td_api::getAuthorizationState>();
            // this->send_query(fn, {});
            // cout << "Error: " << err->message_ << endl;
        }
        catch (exception &e)
        {
            _logger->error("Encountered an error when handling error: {}", e.what());
            return false;
        }
    }
    else
    {
        _logger->debug("Not an error, continuing...");
        return true;
    }
}