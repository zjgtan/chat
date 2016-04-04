#include "client.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string.h>


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
    //建立epoll监听
    _epoll_fd = epoll_create(5);
    //注册网络接口监听
    struct epoll_event ev;
    ev.data.fd = _server_socket;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _server_socket, &ev);
    //注册标准输入监听
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN;
    epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

    return 0;
}


int Client::start()
{
    struct epoll_event events[MAX_EVENTS];
    int nfds;

    //注册
    login();
    while (1)
    {
        nfds = epoll_wait(_epoll_fd, events, MAX_EVENTS);
        for (int i = 0; i < nfds; i++)
        {
            if (events[i].events & EPOLLIN)
            {
                   if (events[i].data.fd == _server_socket)
                {
                    //处理读消息
                    std::string msg;
                    read_msg(msg);
                    // 将消息输出
                    printf("%s\n", msg.c_str());
                }
                else if (events[i].data.fd == STDIN_FILENO)
                {
                    //处理用户输入消息
                    std::string input;
                    getline(std::cin, input);
                    send_msg(input);
                }

            }
    }
    return 0;
}

int Client::read_from_socke_from_sockett(std::string& str)
{
    int count;
    memset(_recv_buf, 0, strlen(_recv_buf));
    count = read(_server_socket, _recv_buf, strlen(_recv_buf));
    str = _recv_buf;

    return count;
}

int Client::login()
{
    printf("username:");
    std::string username;
    getline(std::cin, username);
    std::string msg;
    msg = "login:" + username;
    int count;
    count = write(msg);
    return count;
}

int Client::send_msg(std::string& msg)
{ 
    int count
    count = write(msg);
    return count;
}

int Client::read_msg(std::string& msg)
{
    int count;
    count = read_from_socket(msg);
    return count;
}

int Client::write_to_socket(std::string& buf)
{
    int count;
    count = write(_server_socket, buf.c_str(), strlen(buf.c_str()));
    return count;
}
