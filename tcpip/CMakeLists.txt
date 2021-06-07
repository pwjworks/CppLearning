cmake_minimum_required(VERSION 3.0)

project(tcpip)

set(CMAKE_BUILD_TYPE "Debug")

add_executable(bound_host1 bound_host1.cpp)
add_executable(pipe pipe.cpp)