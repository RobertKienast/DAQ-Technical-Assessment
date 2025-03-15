
cmake_minimum_required(VERSION 3.5)

project(unit_tests LANGUAGES CXX)

add_subdirectory(lib/Catch2)

add_library(decoder ${CMAKE_CURRENT_SOURCE_DIR}/../solution/decoder.cpp)

add_executable(unit_tests main_tests.cpp)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

include_directories(${LIBXML2_INCLUDE_DIR})
target_link_libraries(unit_tests PRIVATE ${LIBXML2_LIBRARIES})


target_link_libraries(unit_tests PRIVATE ${LIBXML2_LIBRARIES})
target_link_libraries(unit_tests PRIVATE dbcppp)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2)
target_link_libraries(unit_tests PRIVATE decoder)
