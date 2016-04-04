#include "server.h"
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
#include <string.h>

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
    options = fcntl(conn_socket, F_GETFL);
    options = options | O_NONBLOCK;
    fcntl(conn_socket, F_SETFL, options);
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
                int client_socket = events[i].data.fd;
                std::string msg;
                read_from_socket(client_socket, msg);
                // 处理读消息
                msg_handle(client_socket, msg);
            }
        }
    }
    return 0;
}

int Server::msg_handle(int client_socket, std::string& msg)
{
    int idx = 0;
    int pos;
    pos = msg.find_first_of(":", idx);
    std::string head = msg.substr(idx, pos - idx);
    if (head.compare("login") == 0)
    {
        //登陆
        idx = pos + 1;
        pos = msg.find_first_of(":", idx);
        std::string username = msg.substr(idx, pos - idx);
        //注册用户
        _client_to_sock.insert(make_pair(username, client_socket));
        _sock_to_client.insert(make_pair(client_socket, username));
        fprintf(stderr, "%s login\n", username.c_str());
    }
    else if (head.compare("msg") == 0)
    {
        //消息
        std::string from, to, body;
        // 获取发送者
        get_user_from_sock(client_socket, from);
        // 获取接收者
        idx = pos + 1;
        pos = msg.find_first_of(":", idx);
        to = msg.substr(idx, pos - idx);

        idx = pos + 1;
        pos = msg.find_first_of(":", idx);
        body = msg.substr(idx, pos - idx);

        printf("%s,%s,%s\n", from.c_str(), to.c_str(), body.c_str());

        if (to.compare("all") == 0)
        {
            int to_sock;
            std::map<int, std::string>::iterator it;
            std::string s_msg = "broadcast:" + from + ":" + body;
            for (it = _sock_to_client.begin(); \
                    it != _sock_to_client.end(); \
                    it ++)
            {
                to_sock = it->first; 
                write_to_socket(to_sock, s_msg);
            }
        }
        else
        {
            int to_sock;
            std::map<std::string, int>::iterator it;
            it = _client_to_sock.find(to);
            if (it == _client_to_sock.end())
                return -1;
            to_sock = it->second;
            std::string s_msg = "msg:" + from + ":" + body;
            write_to_socket(to_sock, s_msg);
        }
    }
}

int Server::get_user_from_sock(int sock_fd, std::string& user)
{
    std::map<int, std::string>::iterator it;
    it = _sock_to_client.find(sock_fd);
    if (it == _sock_to_client.end())
        return -1;
    user = it->second;
}

int Server::read_from_socket(int client_socket, std::string& msg)
{
    ssize_t count;
    //清空接收缓存
    memset(_recv_buf, 0, strlen(_recv_buf));
    count = read(client_socket, _recv_buf, sizeof(_recv_buf));
    msg = _recv_buf;
    return 0;
}

int Server::write_to_socket(int client_socket, std::string& buf)
{
    int count;
    fprintf(stderr, "send: %s\n", buf.c_str());
    count = write(client_socket, buf.c_str(), strlen(buf.c_str()));
    return count;

}
