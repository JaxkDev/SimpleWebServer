# Create the executable
add_executable(SimpleWebServer
    main.cpp
    # Other source files ADD HERE:
    utils.cpp
    server.cpp
)

# ============================================================================
# TARGET-SPECIFIC COMPILE OPTIONS (DO NOT TOUCH)
# ============================================================================

if(MSVC)
    # MSVC compiler options using generator expressions
    target_compile_options(SimpleWebServer PRIVATE
        # Common warnings
        /W4
        /WX
        # Security
        /sdl
        # Language
        /Zc:__cplusplus
        /permissive-
        
        # Configuration-specific options
        $<$<CONFIG:Debug>:
            /Od           # No optimization
            /Zi           # Debug information
            /RTC1         # Runtime checks
        >
        $<$<CONFIG:Release>:
            /O2           # Maximum optimization
            /Ob2          # Inline expansion
            /GF           # String pooling
            /Gy           # Function-level linking
        >
        $<$<CONFIG:RelWithDebInfo>:
            /O2           # Maximum optimization  
            /Zi           # Debug information
            /Ob1          # Inline any suitable
        >
        $<$<CONFIG:MinSizeRel>:
            /O1           # Minimize space
            /Ob1          # Inline any suitable
        >
    )
    
    # MSVC linker options
    target_link_options(SimpleWebServer PRIVATE
        $<$<CONFIG:Debug,RelWithDebInfo>:
            /DEBUG           # Generate debug info
            /INCREMENTAL:NO  # No incremental linking
        >
        $<$<CONFIG:Release,MinSizeRel>:
            /OPT:REF      # Eliminate unused functions
            /OPT:ICF      # Identical COMDAT folding
        >
    )
    
else()
    # GCC/Clang compiler options
    target_compile_options(SimpleWebServer PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        
        # Configuration-specific
        $<$<CONFIG:Debug>:
            -O0
            -g
            -fno-omit-frame-pointer
        >
        $<$<CONFIG:Release>:
            -O3
            -DNDEBUG
            -fomit-frame-pointer
        >
        $<$<CONFIG:RelWithDebInfo>:
            -O2
            -g
            -DNDEBUG
        >
        $<$<CONFIG:MinSizeRel>:
            -Os
            -DNDEBUG
        >
    )
endif()

# Include directories
target_include_directories(SimpleWebServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)


if(WIN32)
    # Windows-specific libraries
    target_link_libraries(SimpleWebServer PRIVATE ws2_32 wsock32)
else()
    # Unix-specific libraries (pthread, dl, etc.)
    target_link_libraries(SimpleWebServer PRIVATE pthread)
endif()

install(TARGETS SimpleWebServer
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)