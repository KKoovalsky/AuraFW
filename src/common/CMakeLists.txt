add_library(aura_common INTERFACE)
target_include_directories(aura_common INTERFACE ${CMAKE_CURRENT_LIST_DIR})

add_executable(aura_simulation main.cpp)
target_link_libraries(aura_simulation aura_common)
