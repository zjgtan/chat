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
    int read(std::string& buf); 
    //写数据
    int write(std::string& buf);
    //登录
    int login(std::string& username);
    //发消息
    int send_msg(std::string& msg);
    //收消息
    int read_msg(std::string& msg);

private:
    // 服务器连接
    int _server_socket;
    char _recv_buf[1024];
};
