if(BUILD_TESTS)
    # Add files to the test executable
    add_executable(SimpleWebServer_UnitTests test_main.cpp test_catch2.cpp)

    # Link against the main SimpleWebServer target
    add_dependencies(SimpleWebServer_UnitTests SimpleWebServer)

    # Link against Catch2 (and any other necessary libraries here, if needed, ensure to add them in libs/CMakeLists.txt)
    target_link_libraries(SimpleWebServer_UnitTests PRIVATE Catch2::Catch2WithMain)

    # Register the tests:
    add_test(NAME SimpleWebServer_UnitTests COMMAND SimpleWebServer_UnitTests)
endif()