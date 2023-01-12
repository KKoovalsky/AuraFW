# =============================================================================
# Macro definitions
# =============================================================================
function(CreateHostTests)

    add_executable(aura_host_tests
        test_measurement_scheduler.cpp
        test_signal_with_compile_time_connections.cpp
    )
    target_link_libraries(aura_host_tests PRIVATE aura_common Catch2::Catch2WithMain jungles::os_helpers)

    add_test(NAME aura_host COMMAND valgrind --leak-check=full $<TARGET_FILE:aura_host_tests>)

endfunction()
# =============================================================================
# Main script
# =============================================================================

ProvideCatch2()
CreateHostTests()
