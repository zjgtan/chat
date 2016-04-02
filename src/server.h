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
    //连接监听
    int _listen_socket;
    //epoll事件触发监听
    int _epoll_fd;
    //读缓冲区
    char _recv_buf[256];


};

#endif
