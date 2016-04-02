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
    //初始化
    int init();
    //启动
    int start();

private:
    //将连接设为非阻塞
    int setnonblocking(int conn_socket);
    //处理消息
    int proc_message(char *buf);

private:
    //主连接
    std::string _ip;
    int _port;
    int _listen_socket;
    int _epoll_fd;


};

#endif
