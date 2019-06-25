#include <vector>
#include <cstdio>
#include <iostream>

#include "VolubleTelegramClientConfig.h"
#include "server/server.h"

using namespace std;

int main()
{
    cout
        << "VTH Version: "
        << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH
        << endl;
    Server svr = Server();
    svr.handle_request();

    return 0;
}