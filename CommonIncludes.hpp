/*
    This file is part of libEpollPlus.
    Copyright (C) 2018 ReimuNotMoe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the MIT License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/


#ifndef LIBEPOLLPLUS_COMMONINCLUDES_HPP
#define LIBEPOLLPLUS_COMMONINCLUDES_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <system_error>

#include <cassert>
#include <cstdlib>
#include <cinttypes>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>

#include <sys/epoll.h>

namespace EpollPlus {
    template<typename T>
    struct is_int : public std::false_type {};
    template<>
    struct is_int<int> : public std::true_type {};
    template<>
    struct is_int<unsigned int> : public std::true_type {};
    template<>
    struct is_int<long> : public std::true_type {};
    template<>
    struct is_int<unsigned long> : public std::true_type {};
    template<>
    struct is_int<short> : public std::true_type {};
    template<>
    struct is_int<unsigned short> : public std::true_type {};
    template<>
    struct is_int<char> : public std::true_type {};
    template<>
    struct is_int<unsigned char> : public std::true_type {};

    struct RawMetaData {
	    int fd;
	    union {
		    void *ptr;
		    uint64_t number;
	    } data;
    };
}

#endif //LIBEPOLLPLUS_COMMONINCLUDES_HPP
