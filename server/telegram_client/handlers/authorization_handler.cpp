#include "authorization_handler.h"

using namespace std;
AuthorizationHandler::AuthorizationHandler(shared_ptr<td::Client> client)
{
    _client = client;
}

void AuthorizationHandler::handle_authorization_state(td::td_api::object_ptr<td::td_api::updateAuthorizationState> &updateAuthState)
{
    switch (updateAuthState->authorization_state_->get_id())
    {
    case td::td_api::authorizationStateWaitTdlibParameters::ID:
    {
        auto authState = td::move_tl_object_as<td::td_api::authorizationStateWaitTdlibParameters>(updateAuthState->authorization_state_);
        cout << "Got AuthStateWaitTdLibParams" << endl;
        handle_tdlib_parameters();
        return;
    }
    case td::td_api::authorizationStateWaitEncryptionKey::ID:
    {
        auto authState = td::move_tl_object_as<td::td_api::authorizationStateWaitEncryptionKey>(updateAuthState->authorization_state_);
        cout << "Got AuthStateWaitEncrKey " << endl;
        handle_wait_encryption_key();
        return;
    }
    }
}

void AuthorizationHandler::handle_tdlib_parameters()
{
    ostringstream version;
    version << VERSION_MAJOR << "." << VERSION_MINOR;
    cout << "handling tdlib params" << endl;
    auto parameters = td::td_api::make_object<td::td_api::tdlibParameters>();
    cout << "made params object" << endl;
    parameters->ignore_file_names_ = false;
    parameters->system_language_code_ = "en-gb";
    parameters->api_hash_ = getenv("TELEGRAM_API_HASH");
    parameters->api_id_ = atoi(getenv("TELEGRAM_API_ID"));
    parameters->application_version_ = version.str();
    parameters->database_directory_ = "";
    parameters->device_model_ = "Asus Zenbook";
    parameters->enable_storage_optimizer_ = false;
    parameters->files_directory_ = "";
    parameters->system_version_ = "";
    parameters->use_chat_info_database_ = true;
    parameters->use_file_database_ = true;
    parameters->use_message_database_ = true;
    parameters->use_secret_chats_ = false;
    parameters->use_test_dc_ = true;
    cout << "Made TdLibParams" << endl;

    auto fn = td::td_api::make_object<td::td_api::setTdlibParameters>(move(parameters));
    cout << "Made function" << endl;
    _client->send({1, move(fn)});
    cout << "send thing" << endl;
}

void AuthorizationHandler::handle_wait_encryption_key()
{
    //shared_ptr<td::Client> client = make_shared<td::Client>();
    auto encryKeyFn = td::make_tl_object<td::td_api::setDatabaseEncryptionKey>("");
    _client->send({2, move(encryKeyFn)});
}