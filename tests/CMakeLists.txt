if(BUILD_TESTS)
    add_executable(SimpleWebServerTests test_main.cpp test_utils.cpp)
    add_dependencies(SimpleWebServerTests SimpleWebServer)
    target_link_libraries(SimpleWebServerTests PRIVATE Catch2::Catch2WithMain)

    add_test(NAME SimpleWebServerTests COMMAND SimpleWebServerTests)
endif()