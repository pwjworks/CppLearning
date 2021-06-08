cmake_minimum_required(VERSION 3.0)

project(tcpip)

set(CMAKE_BUILD_TYPE "Debug")

add_executable(bound_host1 bound_host1.cpp)
add_executable(pipe pipe.cpp)
add_executable(news_receiver news_receiver.cpp)
add_executable(news_sender news_sender.cpp)
add_executable(news_receiver_brd news_receiver_brd.cpp)
add_executable(news_sender_brd news_sender_brd.cpp)
add_executable(sep_clnt sep_clnt.cpp)
add_executable(sep_serv2 sep_serv2.cpp)