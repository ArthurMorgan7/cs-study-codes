cmake_minimum_required(VERSION 3.10)
project(SocketProgrammingExample)

set(CMAKE_CXX_STANDARD 11)


# # 阻塞 IO 示例
# add_executable(blocking_io_server blocking_io_server.cpp)
# target_link_libraries(blocking_io_server pthread)
# add_executable(blocking_io_client blocking_io_client.cpp)
# target_link_libraries(blocking_io_client pthread)


# # 非阻塞 IO 示例
# add_executable(nonblocking_io_server nonblocking_io_server.cpp)
# target_link_libraries(nonblocking_io_server pthread)
# add_executable(nonblocking_io_client nonblocking_io_client.cpp)
# target_link_libraries(nonblocking_io_client pthread)


# # IO 复用示例
# add_executable(io_multiplexing io_multiplexing.cpp)
# target_link_libraries(io_multiplexing pthread)

# 理解HTTP协议
add_executable(http_client http_client.cpp)
add_executable(http_server http_server.cpp)

# epoll
add_executable(epoll_server epoll_server.cpp)
add_executable(epoll_client epoll_client.cpp)
