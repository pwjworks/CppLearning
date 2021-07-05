cmake_minimum_required(VERSION 3.0)

project(cppimpl)

set(CMAKE_BUILD_TYPE "Debug")

add_library(leptjson leptjson.cc)
add_executable(test test.cc)
target_link_libraries(test leptjson)