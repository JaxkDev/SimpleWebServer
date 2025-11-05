# Create the executable
add_executable(SimpleWebServer
    main.cpp
    # Other source files ADD HERE:
    utils.cpp
    server.cpp
)


# Include directories
target_include_directories(SimpleWebServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)


if(WIN32)
    # Windows-specific libraries
    target_link_libraries(SimpleWebServer PRIVATE)
else()
    # Unix-specific libraries (pthread, dl, etc.)
    target_link_libraries(SimpleWebServer PRIVATE)
endif()


# Compiler-specific flags
#if(MSVC)
#    # Visual Studio specific flags
#    target_compile_options(SimpleWebServer PRIVATE /W4 /WX)
#else()
#    # GCC/Clang flags
#    target_compile_options(SimpleWebServer PRIVATE -Wall -Wextra -Wpedantic -Werror)
#endif()


install(TARGETS SimpleWebServer
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)