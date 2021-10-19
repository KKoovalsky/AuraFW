function(EnableClangStaticAnalysis)

    find_program(scan_build NAMES scan-build HINTS ${LLVM_TOOLCHAIN_PATH}/bin/scan-build)

    if(NOT scan_build)
        message(WARNING " scan-build not found! Clang static analysis will be disabled.")
        return()
    endif()

    set(on_side_build_path ${CMAKE_BINARY_DIR}/clang_static_analysis)
    set(reports_path ${CMAKE_BINARY_DIR}/clang_static_analysis_reports)

    # Creates clean directory where the analysis will be built.
    add_custom_target(clang_static_analysis_prepare
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${on_side_build_path}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${on_side_build_path}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    # Runs the analysis from the path created specifically for that task
    add_custom_target(clang_static_analysis
        # scan-build wants Debug build, for better analysis.
        COMMAND ${scan_build} ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} -DCMAKE_BUILD_TYPE=Debug
        COMMAND ${scan_build}
                    -v -v -o ${reports_path} 
                    -enable-checker optin.cplusplus.UninitializedObject 
                    -enable-checker optin.cplusplus.VirtualCall
                    ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${on_side_build_path}
    )
    
    # Run the *_prepare target always before the analysis
    add_dependencies(clang_static_analysis clang_static_analysis_prepare)

    add_test(
        NAME clang_static_analysis
        COMMAND ${CMAKE_COMMAND} --build . --target clang_static_analysis
    )

    set_tests_properties(clang_static_analysis PROPERTIES LABELS StaticCodeAnalysis)

endfunction()

function(FindAndEnableCppCheckOrWarnIfNotFound)

    find_program(cppcheck NAMES cppcheck)
    if(NOT cppcheck)
        message(WARNING "cppcheck not found! Please, install it to enable static analysis")
        return()
    endif()

    add_test(
        NAME cppcheck_static_analysis
        COMMAND ${cppcheck}
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            --enable=warning
            --enable=style
            --enable=performance
            --enable=unusedFunction
            -i${CMAKE_BINARY_DIR}/_deps
            --suppress=preprocessorErrorDirective:*catch2/*
            --error-exitcode=1
    )

    set_tests_properties(cppcheck_static_analysis PROPERTIES LABELS StaticCodeAnalysis)

endfunction()

function(FindAndEnableClangTidyOrWarnIfNotFound)

    set(llvm_toolchain_bin_path ${LLVM_TOOLCHAIN_PATH}/bin/)

    find_program(clang_tidy NAMES clang-tidy HINTS ${llvm_toolchain_bin_path})
    find_program(clang_apply_replacements NAMES clang-apply-replacements HINTS ${llvm_toolchain_bin_path})
    find_program(run_clang_tidy NAMES run-clang-tidy HINTS ${llvm_toolchain_bin_path})

    if(NOT clang_tidy OR NOT clang_apply_replacements OR NOT run_clang_tidy)
        message(WARNING " One of clang-tidy programs not found: "
                        " ${clang_tidy} ${clang_apply_replacements} ${run_clang_tidy}. "
                        " clang-tidy linting is disabled.")
        return()
    endif()


    set(project_sources_regex "(^${PROJECT_ROOT_DIR}/src.*)|(^${PROJECT_ROOT_DIR}/tests.*)")
    add_test(
        NAME clang_tidy_static_analysis
        COMMAND ${run_clang_tidy}
            -clang-tidy-binary ${clang_tidy}
            -clang-apply-replacements-binary ${clang_apply_replacements}
            files ${project_sources_regex}
            -header-filter ${project_sources_regex}
    )

    set_tests_properties(clang_tidy_static_analysis PROPERTIES LABELS StaticCodeAnalysis)

endfunction()

EnableClangStaticAnalysis()
FindAndEnableCppCheckOrWarnIfNotFound()
FindAndEnableClangTidyOrWarnIfNotFound()