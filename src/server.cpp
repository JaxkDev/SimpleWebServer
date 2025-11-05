#include "SimpleWebServer/server.hpp"
#include "SimpleWebServer/utils.hpp"
#include <iostream>
#include <string>
#include <sstream>

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
    server_addr.sin_port = htons((unsigned short)(port_));
    
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
        std::cerr << "Failed to read request: no data received" << std::endl;
        return false;
    }

    // See whats been received here (debug)
    std::cout << "Received request:" << std::endl << buffer << std::endl;

    // Every HTTP protocol starts with a Request-Line
    if (bytes_read < 5) { // Minimum: "GET /"
        //std::cerr << "Invalid request line, too short: " << buffer << std::endl;
        return false;
    }


    if (std::strncmp(buffer, "GET /", 5) != 0) {
        // Only GET method is supported in this simple server (HTTP/0.9)
        //std::cerr << "Unsupported HTTP method in request line: " << buffer << std::endl;
        return false;
    }

    // Split request into lines (replace with more robust parsing as needed, strtok can be used carefully)
    std::istringstream buffer_stream(buffer);
    std::string request_line;
    std::getline(buffer_stream, request_line);

    // Get the method and path (ignoring HTTP/1.0 or HTTP/1.1 that could be at end of line)
    std::istringstream request_line_stream(request_line);
    std::string method;
    std::string path;
    std::string http;
    request_line_stream >> method >> path >> http;

    std::cout << "Method: " << method << ", Path: " << path << std::endl;
    std::cout << "HTTP Version (optional view of future clients): " << http << std::endl;

    if (method != "GET") {
        std::cerr << "Unsupported HTTP method: " << method << std::endl;
        return false;
    }


    std::string response;
    if (path == "/" || path == "/index.html") {
        // Serve index page (or any other static html content requested)
        response = "<html>"
                   "<head><title>Simple Web Server</title></head>"
                   "<body>"
                   "<h1>Welcome to the Simple Web Server!</h1>"
                   "<p>This is the index page.</p>"
                   "</body>"
                   "</html>";
    } else {
        // Basic 404 response for other paths.
        response = "<html>"
                   "File not found"
                   "</html>";
        // Or just return empty content, this is not covered specified in HTTP/0.9 docs.
    }

    // Send on socket
    send(client_socket, response.c_str(), (int)(response.size()), 0);
    return true;
}