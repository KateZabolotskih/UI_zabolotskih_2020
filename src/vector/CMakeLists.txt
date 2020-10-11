# сообщаем о динамической библиотеке и из каких файлов она будет собрана
add_library(vector SHARED
        include/IVector.h
        Vector.cpp
        VectorImpl.cpp)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

# сообщаем какие includ'ы будут видны извне
#target_include_directories(vector PUBLIC include)

set_target_properties(vector PROPERTIES PREFIX "")
set_target_properties(vector PROPERTIES OUTPUT_NAME "vector")

# исполняемые файлы должны перекидываться в bin
set_target_properties(vector PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )

# зависимости этой библиотеки (_logger вряд ли нуждается в такой строчке если вы не реализуете его с использованием чужих библиотек)
target_link_libraries(vector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..//bin/lib/liblogger.dll.a)