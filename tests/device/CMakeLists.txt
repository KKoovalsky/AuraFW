# =============================================================================
# Macro definitions
# =============================================================================

# =============================================================================
# Main script
# =============================================================================

ProvideCatch2()
target_compile_definitions(Catch2 PUBLIC CATCH_CONFIG_NO_POSIX_SIGNALS CATCH_CONFIG_NOSTDOUT CATCH_CONFIG_COLOUR_NONE)

add_executable(dummy_test
    main.cpp catch2_main.cpp
)
MakeDeviceExecutable(dummy_test)
target_link_libraries(dummy_test PRIVATE Catch2::Catch2)
