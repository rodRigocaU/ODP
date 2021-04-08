cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ODP-Protocol)

include_directories(include)

add_executable(client src/client.cpp)
add_executable(server src/server.cpp)