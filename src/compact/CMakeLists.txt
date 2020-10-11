# сообщаем о динамической библиотеке и из каких файлов она будет собрана
add_library(compact SHARED
        Compact.cpp CompactImpl.cpp ../../test/all_test/test.cpp)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

set_target_properties(compact PROPERTIES PREFIX "")
set_target_properties(compact PROPERTIES OUTPUT_NAME "compact")

# исполняемые файлы должны перекидываться в bin
#set_target_properties(compact PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
#        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
#        )

# зависимости этой библиотеки (_logger вряд ли нуждается в такой строчке если вы не реализуете его с использованием чужих библиотек)
target_link_libraries(compact PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/lib/liblogger.dll.a)
target_link_libraries(compact PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/lib/libvector.dll.a)
target_link_libraries(compact PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/lib/libset.dll.a)