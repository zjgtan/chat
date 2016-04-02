#include <stdlib.h>
#include <string>
#include "server.h"

int main(int argc, char *argv[])
{
    std::string ip(argv[1]);
    int port = atoi(argv[2]);

    Server server(ip, port);
    server.init();
    server.start();
}
