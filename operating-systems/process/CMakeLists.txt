cmake_minimum_required(VERSION 3.10)

project(ipc)

set(CMAKE_BUILD_TYPE "Debug")

find_library(RT_LIB rt)

# # 进程间通信 —— 匿名管道
# add_executable(ipc_pipe_unnamed ipc_pipe_unnamed.cpp)

# # 进程间通信 —— 命名管道
# add_executable(ipc_pipe_named ipc_pipe_named.cpp)

# # 进程间通信 —— 消息队列

# add_executable(ipc_mq_posix_send ipc_mq_posix_send.cpp)
# target_link_libraries(ipc_mq_posix_send ${RT_LIB})
# add_executable(ipc_mq_posix_rev ipc_mq_posix_rev.cpp)
# target_link_libraries(ipc_mq_posix_rev ${RT_LIB})


# # 进程间通信 —— 共享内存
# add_executable(ipc_shm_posix_write ipc_shm_posix_write.cpp)
# target_link_libraries(ipc_shm_posix_write ${RT_LIB})
# add_executable(ipc_shm_posix_read ipc_shm_posix_read.cpp)
# target_link_libraries(ipc_shm_posix_read ${RT_LIB})

# # 进程间通信 —— 信号量
# find_package(Threads REQUIRED)
# add_executable(ipc_sem_read ipc_sem_read.cpp)
# target_link_libraries(ipc_sem_read Threads::Threads)
# add_executable(ipc_sem_write ipc_sem_write.cpp)
# target_link_libraries(ipc_sem_write Threads::Threads)


# 进程间通信 —— socket
add_executable(ipc_socket_client ipc_socket_client.cpp)

add_executable(ipc_socket_server ipc_socket_server.cpp)