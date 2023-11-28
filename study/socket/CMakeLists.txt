cmake_minimum_required(VERSION 2.8)

project(socket_demo)

add_executable(server src/server.cc)
add_executable(client src/client.cc)

add_executable(server2 src/server2.cc)
add_executable(client2 src/client2.cc)

add_executable(server3 src/server3.cc)
add_executable(client3 src/client3.cc)

add_executable(server4 src/server4.cc)
add_executable(client4 src/client4.cc)
