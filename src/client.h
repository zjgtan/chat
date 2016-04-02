#include <string>

class Client
{
public:
    Client();
    ~Client();

public:
    int init(std::string ip, int port);
    int start();

private:
    // 服务器连接
    int _server_socket;
};
