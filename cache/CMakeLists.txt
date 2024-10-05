cmake_minimum_required(VERSION 3.20)
project(Cache)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../third_party ${CMAKE_CURRENT_SOURCE_DIR}/build/third_party)

enable_testing()
add_subdirectory(tests)