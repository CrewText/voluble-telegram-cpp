#ifndef SERVER_H
#define SERVER_H
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <string>
#include "telegram_client/telegram_client.h"
#include "database_manager/database_manager.h"

void handle_request();
void handle_get(web::http::http_request request);
#endif