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
add_executable(echo_epollserv echo_epollserv.cpp)
add_executable(echo_EPLTserv echo_EPLTserv.cpp)
add_executable(echo_EPETserv echo_EPETserv.cpp)
add_executable(echo_client echo_client.cpp)
add_executable(thread1 thread1.cpp)
add_executable(thread2 thread2.cpp)
add_executable(thread3 thread3.cpp)
add_executable(thread4 thread4.cpp)


target_link_libraries(thread1 pthread)
target_link_libraries(thread2 pthread)
target_link_libraries(thread3 pthread)
target_link_libraries(thread4 pthread)