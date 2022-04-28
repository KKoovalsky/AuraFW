##########################################################################################
# Functions
##########################################################################################

##########################################################################################
# Main script
##########################################################################################

add_library(serial_logger STATIC serial_logger.cpp)
target_include_directories(serial_logger PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(serial_logger PUBLIC aura_common)
target_link_libraries(serial_logger PRIVATE cube)

ProvidePrintfLibrary()
target_link_libraries(serial_logger PRIVATE printf)
