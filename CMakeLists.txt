cmake_minimum_required(VERSION 3.8)
project(libEpollPlus)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp EpollPlus.cpp EpollPlus.hpp CommonIncludes.hpp EpollEvent.hpp Epoll.hpp)
add_executable(libEpollPlus ${SOURCE_FILES})