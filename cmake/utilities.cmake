macro(EnableClangStaticAnalysis)

    set(on_side_build_path ${CMAKE_BINARY_DIR}/clang_static_analysis)
    set(scan_build_path ${LLVM_TOOLCHAIN_PATH}/bin/scan-build)
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
        COMMAND ${scan_build_path} ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} -DCMAKE_BUILD_TYPE=Debug
        COMMAND ${scan_build_path}
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

endmacro()

function(FindAndEnableCppCheckOrWarnIfNotFound)

    find_program(CPPCHECK NAMES cppcheck)
    if(NOT CPPCHECK)
        message(WARNING "cppcheck not found! Please, install it to enable static analysis")
        return()
    endif()

    add_test(
        NAME cppcheck_static_analysis
        COMMAND ${CPPCHECK} 
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            --enable=warning
            --enable=style
            --enable=performance
            --enable=unusedFunction
            -i${CMAKE_BINARY_DIR}/_deps
            --suppress=preprocessorErrorDirective:*catch2/*
            --error-exitcode=1
    )

endfunction()

function(FindAndEnableClangTidyOrWarnIfNotFound)

    set(llvm_toolchain_bin_path ${LLVM_TOOLCHAIN_PATH}/bin/)

    find_program(CLANG_TIDY NAMES clang-tidy HINTS ${llvm_toolchain_bin_path})
    find_program(CLANG_APPLY_REPLACEMENTS NAMES clang-apply-replacements HINTS ${llvm_toolchain_bin_path})
    find_program(RUN_CLANG_TIDY NAMES run-clang-tidy HINTS ${llvm_toolchain_bin_path})

    if(NOT CLANG_TIDY OR NOT CLANG_APPLY_REPLACEMENTS OR NOT RUN_CLANG_TIDY)
        message(WARNING " One of clang-tidy programs not found: "
                        " ${CLANG_TIDY} ${CLANG_APPLY_REPLACEMENTS} ${RUN_CLANG_TIDY}. "
                        " clang-tidy linting is disabled.")
        return()
    endif()


    set(project_sources_regex "(^${PROJECT_ROOT_DIR}/src.*)|(^${PROJECT_ROOT_DIR}/tests.*)")
    add_test(
        NAME clang_tidy_static_analysis
        COMMAND ${RUN_CLANG_TIDY}
            -clang-tidy-binary ${CLANG_TIDY}
            -clang-apply-replacements-binary ${CLANG_APPLY_REPLACEMENTS}
            files ${project_sources_regex}
            -header-filter ${project_sources_regex}
    )

endfunction()

EnableClangStaticAnalysis()
FindAndEnableCppCheckOrWarnIfNotFound()
FindAndEnableClangTidyOrWarnIfNotFound()