#include "SimpleWebServer/server.hpp"
#include "SimpleWebServer/utils.hpp"
#include <iostream>
#include <string>

#ifdef _WIN32
    #define CLOSE_SOCKET closesocket
#else
    #define CLOSE_SOCKET close
#endif

Server::Server(int port) : port_(port), running_(false) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

Server::~Server() {
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

bool Server::start() {
    // Create socket
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) {
        std::cerr << "Failed to create socket\n";
        return false;
    }
    
    // Set socket options
    int opt = 1;
#ifdef _WIN32
    setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
    setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
    
    // Bind socket
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);
    
    if (bind(server_socket_, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed\n";
        return false;
    }
    
    // Listen for connections
    if (listen(server_socket_, 10) < 0) {
        std::cerr << "Listen failed\n";
        return false;
    }
    
    std::cout << "Simple Web Server started on " << SimpleWebServer::getPlatformName() << "\n";
    std::cout << "Server running on http://localhost:" << port_ << "\n";
    
    running_ = true;
    
    // Main server loop, one connection at a time
    while (running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        
        // Accept connection (BLOCKING)
        socket_t client_socket = accept(server_socket_, (sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }
        
        // Handle the client request
        handleConnection(client_socket);
        
        // Close client socket
        CLOSE_SOCKET(client_socket);
    }
    
    return true;
}

void Server::stop() {
    running_ = false;
    if (server_socket_ >= 0) {
        CLOSE_SOCKET(server_socket_);
    }
}

bool Server::handleConnection(socket_t client_socket) {
    char buffer[4096] = {0};
    
    // Read from socket
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        return false;
    }
    
    // See whats been received here (debug)
    std::cout << "Received request:\n" << buffer << "\n";
    
    // Demo response
    std::string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "Set-Cookie: demo_cookie=SimpleWebServer; HttpOnly; SameSite=Strict; Secure\r\n"
        "\r\n"
        "<html>"
        "<head><title>Simple Web Server</title></head>"
        "<body>"
        "<h1>Hello from Simple Web Server!</h1>"
        "<p>This is a minimal web server without event polling.</p>"
        "</body>"
        "</html>";
    
    // Send on socket
    send(client_socket, response.c_str(), response.size(), 0);
    
    return true;
}