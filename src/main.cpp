#include "SimpleWebServer/utils.hpp"
#include <iostream>

int main() {
    std::cout << "Running on: " << SimpleWebServer::getPlatformInfo() << std::endl;
    std::cout << "Hello World!" << std::endl;
    return 0;
}