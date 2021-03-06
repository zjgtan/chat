#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <map>

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
    int msg_handle(int client_socket, std::string& msg);
    //底层通信接口
    //向客户端发送消息
    int write_to_socket(int client_socket, std::string& buf);
    //接收服务端
    int read_from_socket(int client_socket, std::string& buf);
    int get_user_from_sock(int sock_fd, std::string& user);

private:
    //主连接
    std::string _ip;
    int _port;
    //连接监听
    int _listen_socket;
    //epoll事件触发监听
    int _epoll_fd;
    //读缓冲区
    char _recv_buf[1024];
    //用户名与连接描述符映射词典
    std::map<std::string, int> _client_to_sock;
    std::map<int, std::string> _sock_to_client;
};

#endif
