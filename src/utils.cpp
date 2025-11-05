#include "SimpleWebServer/utils.hpp"
#include <string>

// Use standard compiler-defined macros
#ifdef _WIN32
    #include <windows.h>
    #define SWS_SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SWS_SLEEP(ms) usleep((ms) * 1000)
#endif

namespace SimpleWebServer {
    std::string getPlatformInfo() {
        #ifdef _WIN32
            return "Windows";
        #else
            return "Unix?";
        #endif
    }
    
    void crossPlatformFunction() {
        // Cross-platform implementation
        SWS_SLEEP(1000);  // Sleep for 1 second
    }
}