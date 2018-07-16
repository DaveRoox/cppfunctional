cmake_minimum_required(VERSION 3.10)
project(functional_list)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp functional/functional.hpp)

include_directories(functional)

add_subdirectory(functional)
add_subdirectory(functional_tests)

target_link_libraries(main ${functional})