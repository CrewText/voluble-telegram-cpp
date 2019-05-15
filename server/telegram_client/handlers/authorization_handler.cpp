#include "authorization_handler.h"

using namespace std;
AuthorizationHandler::AuthorizationHandler(shared_ptr<td::Client> client)
{
    _client = client;
    _logger = spdlog::get("console");
    if (_logger == nullptr)
    {
        cout << "AuthHandler: No logger found!" << endl;
    }
}

AuthorizationHandler::~AuthorizationHandler()
{
    _client = nullptr;
}

td::td_api::object_ptr<td::td_api::Function> AuthorizationHandler::handle_authorization_state(td::td_api::object_ptr<td::td_api::updateAuthorizationState> &updateAuthState)
{
    switch (updateAuthState->authorization_state_->get_id())
    {
    case td::td_api::authorizationStateWaitTdlibParameters::ID:
    {
        auto authState = td::td_api::move_object_as<td::td_api::authorizationStateWaitTdlibParameters>(updateAuthState->authorization_state_);
        _logger->debug("AuthHandler: Got AuthStateWaitTdLibParams");
        return handle_tdlib_parameters();
    }
    case td::td_api::authorizationStateWaitEncryptionKey::ID:
    {
        auto authState = td::td_api::move_object_as<td::td_api::authorizationStateWaitEncryptionKey>(updateAuthState->authorization_state_);
        _logger->debug("AuthHandler: Got AuthStateWaitEncrKey");
        return handle_wait_encryption_key();
    }
    case td::td_api::authorizationStateWaitPhoneNumber::ID:
    {
        auto authState = td::td_api::move_object_as<td::td_api::authorizationStateWaitPhoneNumber>(updateAuthState->authorization_state_);
        _logger->debug("AuthHandler: Got AuthStateWaitPhoneNumber");
        return handle_wait_phone_number();
    }
    case td::td_api::authorizationStateWaitCode::ID:
    {
        auto authState = td::td_api::move_object_as<td::td_api::authorizationStateWaitCode>(updateAuthState->authorization_state_);
        _logger->debug("AuthHandler: Got AuthStateWaitCode; Is registered? {}", authState->is_registered_);

        cout << "ToS: " << endl
             << &authState->terms_of_service_->text_->text_ << endl;
        return handle_wait_code();
    }
    case td::td_api::authorizationStateReady::ID:
    {
        _logger->debug("Got AuthStateReady");
        return nullptr;
    }
    default:
    {
        _logger->error("AuthHandler: Got an unexpected AuthState, ID: {}", updateAuthState->authorization_state_->get_id());
        assert(false);
    }
    }
}

td::td_api::object_ptr<td::td_api::Function> AuthorizationHandler::handle_tdlib_parameters()
{
    ostringstream version;
    version << VERSION_MAJOR << "." << VERSION_MINOR;
    // cout << "handling tdlib params" << endl;
    auto parameters = td::td_api::make_object<td::td_api::tdlibParameters>();
    // cout << "made params object" << endl;
    parameters->ignore_file_names_ = false;
    parameters->system_language_code_ = "en-gb";
    parameters->api_hash_ = getenv("TELEGRAM_API_HASH");
    parameters->api_id_ = atoi(getenv("TELEGRAM_API_ID"));
    parameters->application_version_ = version.str();
    parameters->database_directory_ = "";
    parameters->device_model_ = "Asus Zenbook";
    parameters->enable_storage_optimizer_ = false;
    parameters->files_directory_ = "";
    parameters->system_version_ = "Win 10";
    parameters->use_chat_info_database_ = true;
    parameters->use_file_database_ = true;
    parameters->use_message_database_ = true;
    parameters->use_secret_chats_ = false;
    parameters->use_test_dc_ = true;
    // cout << "Made TdLibParams" << endl;

    //_client->send({1, td::td_api::make_object<td::td_api::setTdlibParameters>(move(parameters))});
    //_client->cout << "send thing" << endl;
    return td::td_api::make_object<td::td_api::setTdlibParameters>(move(parameters));
}

td::td_api::object_ptr<td::td_api::Function> AuthorizationHandler::handle_wait_encryption_key()
{
    return td::td_api::make_object<td::td_api::setDatabaseEncryptionKey>("");
}

td::td_api::object_ptr<td::td_api::Function> AuthorizationHandler::handle_wait_phone_number()
{
    return td::td_api::make_object<td::td_api::setAuthenticationPhoneNumber>("+447426437449", false, true);
}

td::td_api::object_ptr<td::td_api::Function> AuthorizationHandler::handle_wait_code()
{
    string code;
    getline(cin, code);
    return td::td_api::make_object<td::td_api::checkAuthenticationCode>(code, "Cal", "McLean");
}