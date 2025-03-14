include_directories(${LIBXML2_INCLUDE_DIR})
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/dbcppp ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/dbcppp/build)

add_library(decoder STATIC ${CMAKE_CURRENT_SOURCE_DIR}/../decoder.cpp)
add_library(decoder_tests OBJECT main_tests.cpp)

#target_include_directories(decoder_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/dbcppp/include)
target_link_libraries(decoder_tests PRIVATE ${LIBXML2_LIBRARIES})
target_link_libraries(decoder_tests PRIVATE dbcppp)
target_link_libraries(decoder_tests PRIVATE Catch2::Catch2)
target_link_libraries(decoder_tests PRIVATE decoder)

