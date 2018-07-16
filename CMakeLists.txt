cmake_minimum_required(VERSION 3.10)
project(functional_list)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp cppfunctional/functional.hpp)

include_directories(cppfunctional)

add_subdirectory(cppfunctional)
add_subdirectory(cppfunctional_tests)

target_link_libraries(main ${functional})