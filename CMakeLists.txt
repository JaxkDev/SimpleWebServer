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

# ============================================================================
# COMPILER-SPECIFIC SETTINGS FOR DIFFERENT BUILD TYPES
# ============================================================================

# Common flags for all build types
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# MSVC-specific settings
if(MSVC)
    # Disable SAFESEH for better compatibility
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    
    # Debug configuration
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi /RTC1 /MDd")
    # Release configuration  
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /DNDEBUG /MD")
    # RelWithDebInfo configuration
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /O2 /Ob1 /Zi /DNDEBUG /MD")
    # MinSizeRel configuration
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /O1 /Ob1 /DNDEBUG /MD")
    
    # Linker flags for debug info
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DEBUG /INCREMENTAL:NO")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG /INCREMENTAL:NO")
    
else()
    # GCC/Clang configurations
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
endif()


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