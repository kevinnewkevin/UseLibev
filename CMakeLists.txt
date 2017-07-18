cmake_minimum_required(VERSION 3.7)
project(UseLibev)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
link_directories(lib)

set(SOURCE_FILES main.cpp Example.cpp Example.h)
add_executable(UseLibev ${SOURCE_FILES})