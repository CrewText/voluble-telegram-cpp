#include "option_handler.h"

using namespace std;

OptionHandler::OptionHandler(std::shared_ptr<td::Client> client)
{
    _client = client;
    _logger = spdlog::get("console");
    if (_logger == nullptr)
    {
        cout << "OptHandler: No logger found!" << endl;
    }
}

OptionHandler::~OptionHandler()
{
    _client = nullptr;
}

void OptionHandler::handle_option(td::td_api::object_ptr<td::td_api::updateOption> &updateOption)
{
    switch (updateOption->value_->get_id())
    {
    case td::td_api::optionValueEmpty::ID:
    {
        // Unnecessary!
        // auto option_value = td::move_tl_object_as<td::td_api::optionValueEmpty>(updateOption->value_);
        _logger->debug("OptHandler: Got empty option {}", updateOption->name_);
        // cout << "Got empty option " << updateOption->name_ << endl;
        return;
    }
    case td::td_api::optionValueBoolean::ID:
    {
        auto option_value = td::td_api::move_object_as<td::td_api::optionValueBoolean>(updateOption->value_);
        _logger->debug("OptHandler: Got option {}: {}", updateOption->name_, option_value->value_);
        // cout
        //     << "Got option '" << updateOption->name_ << "': " << option_value->value_ << endl;
        break;
    }
    case td::td_api::optionValueString::ID:
    {
        auto option_value = td::td_api::move_object_as<td::td_api::optionValueString>(updateOption->value_);
        _logger->debug("OptHandler: Got option {}: {}", updateOption->name_, option_value->value_);
        // cout << "Got option '" << updateOption->name_ << "': " << option_value->value_ << endl;
        break;
    }
    case td::td_api::optionValueInteger::ID:
    {
        auto option_value = td::td_api::move_object_as<td::td_api::optionValueInteger>(updateOption->value_);
        _logger->debug("OptHandler: Got option {}: {}", updateOption->name_, option_value->value_);
        // cout << "Got option '" << updateOption->name_ << "': " << option_value->value_ << endl;
        break;
    }
    default:
    {
        _logger->error("OptHandler: Got an unexpected OptionValue, ID: {}", updateOption->value_->get_id());
        assert(false);
    }
    }
}