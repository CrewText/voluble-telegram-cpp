# Voluble Telegram Client
This is a standalone Telegram client, intended to be used as a gateway between [Voluble](https://github.com/calmcl1/voluble) and Telegram. 

The client works by logging in as a Telegram user (on behalf of a Voluble Organization) and waiting for inbound messages to report to Voluble, or outbound messages from Voluble to send to the specified Telegram users.

## Environment Variables
VTC depends on the following Environment Variables in order to run:

| Variable Name | Usage | Required |
|---|---|---|
| TELEGRAM_API_ID | The Telegram API ID that the client will use to interact with the Telegram API. | YES |
| TELEGRAM_API_HASH | The Telegram API Hash that the client will use to interact with the Telegram API. | YES |
| TELEGRAM_DB_LOCATION | The location of the database in which to store internal VTC settings. | YES |
| TD_LOG_PATH | The location of the log file to write TD information to. If not supplied, TD will write to stdout. | NO |