#pragma once

#include <string>

namespace SimpleWebServer {
    std::string getPlatformName();
    void sleep(int seconds = 1);
};