cmake_minimum_required (VERSION 3.14)
project (leptjson_test C)

set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall")
endif()

add_library(leptjson leptjson.c)
add_executable(test test.c)
target_link_libraries(test leptjson)