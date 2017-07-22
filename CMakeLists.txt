cmake_minimum_required(VERSION 3.7)
project(UseLibev)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
link_directories(lib)

set(SOURCE_FILES main.cpp easylogging++.cc easylogging++.h Client.cpp Client.h ChatRoom.cpp ChatRoom.h)
add_executable(UseLibev ${SOURCE_FILES})