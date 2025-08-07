cmake_minimum_required(VERSION 3.10)

project(SingletonExample)

set(CMAKE_CXX_STANDARD 20)

add_executable(singleton singleton.cpp)

add_executable(look look.cpp)

add_executable(adapt adapt.cpp)