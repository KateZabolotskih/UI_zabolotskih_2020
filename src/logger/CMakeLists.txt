# сообщаем о динамической библиотеке и из каких файлов она будет собрана
add_library(logger SHARED
        LoggerImpl.cpp Logger.cpp)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

set_target_properties(logger PROPERTIES PREFIX "")
set_target_properties(logger PROPERTIES OUTPUT_NAME "logger")

# исполняемые файлы должны перекидываться в bin
#set_target_properties(logger PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
#        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
#        )