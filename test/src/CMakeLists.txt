add_executable(test test.cpp)

target_link_libraries(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/liblogger.dll.a)
target_link_libraries(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libvector.dll.a)