add_executable(test test.cpp vector.cpp set.cpp compact.cpp)


target_link_libraries(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/liblogger.dll.a)
target_link_libraries(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libvector.dll.a)
target_link_libraries(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libset.dll.a)
target_link_libraries(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/libcompact.dll.a)