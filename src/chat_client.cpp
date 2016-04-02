#include "client.h"
#include <string>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    Client client;
    std::string ip = argv[1];
    int port = atoi(argv[2]);
    client.init(ip, port);
    client.start();
}
