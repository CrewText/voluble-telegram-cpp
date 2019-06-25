#include "option_handler.h"

using namespace std;

OptionHandler::OptionHandler(std::shared_ptr<td::Client> client)
{
    _client = client;
    _logger = create_logger("OptionHandler");
    if (_logger == nullptr)
    {
        cout << "OptionHandler: No logger found!" << endl;
    }
}

OptionHandler::~OptionHandler()
{
    _client = nullptr;
}

void OptionHandler::handle_option(td::td_api::object_ptr<td::td_api::updateOption> &updateOption)
{

    if (updateOption == nullptr)
    {
        _logger->warn("updateOption is null!");
        return;
    }
    switch (updateOption->value_->get_id())
    {
    case td::td_api::optionValueEmpty::ID:
    {
        _logger->debug("Got empty option {}", updateOption->name_);
        return;
    }
    case td::td_api::optionValueBoolean::ID:
    {
        auto option_value = td::td_api::move_object_as<td::td_api::optionValueBoolean>(updateOption->value_);
        _logger->debug("Got option {}: {}", updateOption->name_, option_value->value_);
        break;
    }
    case td::td_api::optionValueString::ID:
    {
        auto option_value = td::td_api::move_object_as<td::td_api::optionValueString>(updateOption->value_);
        _logger->debug("Got option {}: {}", updateOption->name_, option_value->value_);
        break;
    }
    case td::td_api::optionValueInteger::ID:
    {
        auto option_value = td::td_api::move_object_as<td::td_api::optionValueInteger>(updateOption->value_);
        _logger->debug("Got option {}: {}", updateOption->name_, option_value->value_);
        break;
    }
    default:
    {
        _logger->error("Got an unexpected OptionValue, ID: {}", updateOption->value_->get_id());
        assert(false);
    }
    }
}