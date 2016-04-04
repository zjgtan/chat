#include <string>

#define MAX_EVENTS 20

class Client
{
public:
    Client();
    ~Client();

public:
    int init(std::string ip, int port);
    int start();
    //读数据
    int read_from_socket(std::string& buf); 
    //写数据
    int write_to_socket(std::string& buf);
    //登录
    int login();
    //发消息
    int send_msg(std::string& msg);
    //收消息
    int read_msg(std::string& msg);

private:
    // 服务器连接
    int _server_socket;
    // epoll监听
    int _epoll_fd;
    char _recv_buf[1024];
};
