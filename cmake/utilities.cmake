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

EnableClangStaticAnalysis()
