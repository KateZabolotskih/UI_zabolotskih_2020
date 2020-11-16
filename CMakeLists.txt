cmake_minimum_required(VERSION 3.17)
project(I_zabolotskih_2020)

# это важно
set(CMAKE_C_FLAGS -m32)
set(CMAKE_CXX_FLAGS -m32)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# тесты тут
add_subdirectory(test)
#add_subdirectory(mark_test)
#add_subdirectory(petrunin_test)

# исходный код тут
add_subdirectory(src)