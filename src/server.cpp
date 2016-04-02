#include "server.h"
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

Server::Server()
{}

Server::Server(std::string ip, int port) : _ip(ip), _port(port)
{}

Server::~Server()
{
}

int Server::setnonblocking(int conn_socket)
{
    int options;
    options = fcntl(sock, F_GETFL);
    options = options | O_NONBLOCK;
    fcntl(conn_socket, F_SETFL, options);
    return 0;
}

int Server::proc_message(char *buf)
{  
    printf("%s\n", buf);
    return 0;
}

int Server::init()
{
    //监听端口
    struct sockaddr_in server_addr;
    _listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    //初始化连接信息
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_aton(_ip.c_str(), &(server_addr.sin_addr));
    server_addr.sin_port = htons(_port);
    bind(_listen_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(_listen_socket, 20);

    //初始化epoll
    _epoll_fd = epoll_create(10);
    struct epoll_event ev;
    ev.data.fd = _listen_socket;
    ev.events = EPOLLIN;
    epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _listen_socket, &ev);

    return 0;
}

int Server::start()
{
    int nfds;
    int conn_socket;
    struct sockaddr_in client_addr;
    socklen_t addrlen;
    struct epoll_event events[MAX_EVENTS];
    struct epoll_event ev;

    while (1)
    {
        nfds = epoll_wait(_epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == _listen_socket)
            {
                conn_socket = accept(_listen_socket, (struct sockaddr *)&client_addr, &addrlen);    
                setnonblocking(conn_socket);

                //添加事件监听
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_socket;
                epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, conn_socket, &ev);

                //添加客户端连接到全局词典
            }
            else if (events[i].events & EPOLLIN)
            {
                // 处理读消息
                int conn_socket = events[i].data.fd;
                ssize_t count;
                //清空接收缓存
                memset(_recv_buf, 0, strlen(_recv_buf));
                count = read(conn_socket, _recv_buf, sizeof(_recv_buf));
                proc_message(_recv_buf);
            }
        }
    }
    return 0;
}
