#include "SimpleWebServer/utils.hpp"
#include "SimpleWebServer/server.hpp"
#include <iostream>

int main() {
    Server server(80);
    
    if (!server.start()) {
        std::cerr << "Failed to start server\n";
        return 1;
    }
    
    return 0;
} 