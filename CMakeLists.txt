cmake_minimum_required(VERSION 2.8)
project(libEpollPlus)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES EpollPlus.cpp EpollPlus.hpp CommonIncludes.hpp EpollEvent.hpp Epoll.hpp)
add_library(EpollPlus ${SOURCE_FILES})


add_executable(EpollPlus_test test.cpp)

install(FILES Epoll.hpp EpollEvent.hpp EpollPlus.hpp CommonIncludes.hpp DESTINATION include/EpollPlus/)