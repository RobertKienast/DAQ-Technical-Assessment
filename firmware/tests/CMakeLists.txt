
cmake_minimum_required(VERSION 3.5)

project(unit_tests LANGUAGES CXX)



add_subdirectory(lib/Catch2)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../modules/dbcppp ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dbcppp/build/CMakeFiles)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../solution/main_unit_tests main_tests)
add_executable(unit_tests)
include_directories(${LIBXML2_INCLUDE_DIR})
target_link_libraries(unit_tests PRIVATE ${LIBXML2_LIBRARIES})
#target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../modules/dbcppp)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(unit_tests PRIVATE decoder_tests)
target_link_libraries(unit_tests PRIVATE dbcppp)
#${CMAKE_CURRENT_SOURCE_DIR}/../modules/dbcppp/build
