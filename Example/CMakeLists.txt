cmake_minimum_required(VERSION 3.10)

project(Example)

include_directories(..)

add_executable(Example main.cpp)

target_link_libraries(Example MemX)
