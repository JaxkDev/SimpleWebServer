#pragma once

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") // Link with Winsock 2 library
    using socket_t = SOCKET;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    using socket_t = int;
#endif

#include <string>

class Server {
public:
    Server(int port = 80);
    ~Server();

    bool start();
    void stop();
    
private:
    bool handleConnection(socket_t client_socket);
    std::string createHttpResponse(const std::string& content);
    
    socket_t server_socket_;
    int port_;
    bool running_;
};