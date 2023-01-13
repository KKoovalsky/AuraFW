# =============================================================================
# Macro definitions
# =============================================================================
function(CreateHostTests)

    add_executable(aura_host_tests
        test_measurement_scheduler.cpp
        test_signal_with_compile_time_connections.cpp
        test_active_with_compile_time_actors.cpp
    )
    target_link_libraries(aura_host_tests PRIVATE aura_common Catch2::Catch2WithMain jungles::os_helpers)

    add_test(NAME aura_host COMMAND valgrind --leak-check=full $<TARGET_FILE:aura_host_tests>)

endfunction()

function(CreateBenchmark name source_file)

    add_executable(${name} ${source_file})
    target_link_libraries(${name} PRIVATE aura_common Catch2::Catch2WithMain)
    add_custom_target(${name}-benchmark COMMAND $<TARGET_FILE:${name}>)

endfunction()
# =============================================================================
# Main script
# =============================================================================

ProvideCatch2()
CreateHostTests()

CreateBenchmark(signal_std_array_based benchmark_signal_std_array_based.cpp)
CreateBenchmark(signal_std_tuple_based benchmark_signal_std_tuple_based.cpp)
