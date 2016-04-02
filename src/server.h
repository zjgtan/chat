#ifndef SERVER_H_
#define SERVER_H_

#include <string>

#define MAX_EVENTS 10

class Server
{
public:
    Server();
    Server(std::string ip, int port);
    ~Server();
public:
    int init();
    int start();

private:
    //主连接
    std::string _ip;
    int _port;
    int _listen_socket;
    int _epoll_fd;


};

#endif
