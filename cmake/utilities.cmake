macro(EnableClangStaticAnalysis)

    add_test(
        # Rebuild the entire, cleaned project on-the-side with scan-build.
        NAME clang_static_analysis 
        COMMAND ${CMAKE_CTEST_COMMAND} 
            # 'Build myself' by taking the main CMakeLists.txt
            --build-and-test ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/clang_static_analysis
            # This is a hack: effectively, will skip the Build stage by not performing implicit clean, but
            # we invoke it anyway. We could not use '--build-noclean', but then 'clean' would be called twice.
            --build-noclean
            --build-target clean
            --build-generator ${CMAKE_GENERATOR}
            # scan-build needs Debug configuration
            --build-options -DCMAKE_BUILD_TYPE=Debug
            --test-command 
                ${LLVM_TOOLCHAIN_PATH}/bin/scan-build 
                    -v -v -o reports 
                    -enable-checker optin.cplusplus.UninitializedObject 
                    -enable-checker optin.cplusplus.VirtualCall
                    ${CMAKE_COMMAND} --build .
    )

endmacro()

EnableClangStaticAnalysis()
