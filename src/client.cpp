#include "client.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

Client::Client()
{

}

Client::~Client()
{

}

int Client::init(std::string ip, int port)
{
    struct sockaddr_in server_addr;
    //连接服务器
    _server_socket = socket(AF_INET, SOCK_STREAM, 0);
    //设置服务器地址
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    //连接
    connect(_server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    return 0;
}

int Client::start()
{
    return 0; 
}
