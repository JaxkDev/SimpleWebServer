cmake_minimum_required(VERSION 3.15)
project(SimpleWebServer VERSION 1.0.0 LANGUAGES CXX)


# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Platform
if(WIN32)
    add_compile_definitions(SimpleWebServer_WIN32)
    # Windows-specific settings here
else()
    add_compile_definitions(SimpleWebServer_UNIX)
    # Unix-specific settings here
endif()


# Build configuration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# Other build options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTS "Build tests" ON)


# External libraries and dependencies
add_subdirectory(libs)


# Source directories
add_subdirectory(src)


# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()