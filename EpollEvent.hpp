/*
    This file is part of libEpollPlus.
    Copyright (C) 2018 ReimuNotMoe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the MIT License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef LIBEPOLLPLUS_EPOLLEVENT_HPP
#define LIBEPOLLPLUS_EPOLLEVENT_HPP

#include "CommonIncludes.hpp"

namespace EpollPlus {

    template<class T, typename = void>
    class EpollEvent {
    private:
	int fd = -1;
	uint32_t events = 0;
	std::shared_ptr<RawMetaData> sptr_raw_metadata;

    public:
	EpollEvent() = default;

	EpollEvent(const uint32_t __events, const std::shared_ptr<RawMetaData> &__sptr_rawmetadata) {
		events = __events;
		sptr_raw_metadata = __sptr_rawmetadata;
		fd = (*sptr_raw_metadata).fd;
	}

	EpollEvent(int user_fd, T *user_data, uint32_t __events) {
		fd = user_fd;
		events = __events;

		sptr_raw_metadata = std::make_shared<RawMetaData>();
		auto raw_metadata = sptr_raw_metadata.get();

		raw_metadata->fd = fd;
		raw_metadata->data.ptr = user_data;
	}

	T* const UserData() const {
		return reinterpret_cast<T *>((*sptr_raw_metadata).data.ptr);
	}

	EpollEvent &SetUserData(T *ptr) {
		(*sptr_raw_metadata).data.ptr = ptr;
		return *this;
	}

	int FD() const {
		return fd;
	}

	EpollEvent &SetFD(int __fd) {
		fd = __fd;
		return *this;
	}

	const std::shared_ptr<RawMetaData> &RawUserData () const {
		return sptr_raw_metadata;
	}

	std::shared_ptr<epoll_event> RawEvent() const {
		auto ret = std::make_shared<epoll_event>();

		(*ret).events = events;
		(*ret).data.ptr = sptr_raw_metadata.get();

		return ret;
	};

	const uint32_t Events() const {
		return events;
	}

	EpollEvent &SetEvents(uint32_t __events) {
		events = __events;
		return *this;
	}

	uint32_t operator& (uint32_t rhs) const {
		return events & rhs;
	}

	uint32_t operator| (uint32_t rhs) const {
		return events | rhs;
	}

	uint32_t operator&= (uint32_t rhs) {
		return (events &= rhs);
	}

	uint32_t operator|= (uint32_t rhs) {
		return (events |= rhs);
	}

	bool operator== (const EpollEvent& rhs) const {
		return (events == rhs.events) && (sptr_raw_metadata == rhs.sptr_raw_metadata);
	}

	bool operator!= (const EpollEvent& rhs) const {
		return (events != rhs.events) || (sptr_raw_metadata != rhs.sptr_raw_metadata);
	}

	void aaa() {
		printf("Using class T\n");
	}
    };


    template<class T>
    class EpollEvent<T, typename std::enable_if<is_int<T>::value>::type> {
    private:
	int fd = -1;
	uint32_t events = 0;
	std::shared_ptr<RawMetaData> sptr_raw_metadata;

    public:
	EpollEvent() = default;

	EpollEvent(const uint32_t __events, const std::shared_ptr<RawMetaData> &__sptr_rawmetadata) {
		events = __events;
		sptr_raw_metadata = __sptr_rawmetadata;
		fd = (*sptr_raw_metadata).fd;
	}

	EpollEvent(int user_fd, T user_data, uint32_t __events) {
		fd = user_fd;
		events = __events;

		sptr_raw_metadata = std::make_shared<RawMetaData>();
		auto raw_metadata = sptr_raw_metadata.get();

		raw_metadata->fd = fd;
		memcpy(&(raw_metadata->data.number), &user_data, sizeof(T));
	}

	T const UserData() const {
		T ret;
		memcpy(&ret, &(*sptr_raw_metadata).data.number, sizeof(T));
		return ret;
	}

	EpollEvent &SetUserData(T val) {
		memcpy(&(*sptr_raw_metadata).data.number, &val, sizeof(T));
		return *this;
	}

	int FD() const {
		return fd;
	}

	EpollEvent &SetFD(int __fd) {
		fd = __fd;
		return *this;
	}

	const std::shared_ptr<RawMetaData> &RawUserData () const {
		return sptr_raw_metadata;
	}

	std::shared_ptr<epoll_event> RawEvent() const {
		auto ret = std::make_shared<epoll_event>();

		(*ret).events = events;
		(*ret).data.ptr = sptr_raw_metadata.get();

		return ret;
	};

	const uint32_t Events() const {
		return events;
	}

	EpollEvent &SetEvents(uint32_t __events) {
		events = __events;
		return *this;
	}

	uint32_t operator& (uint32_t rhs) const {
		return events & rhs;
	}

	uint32_t operator| (uint32_t rhs) const {
		return events | rhs;
	}

	uint32_t operator&= (uint32_t rhs) {
		return (events &= rhs);
	}

	uint32_t operator|= (uint32_t rhs) {
		return (events |= rhs);
	}

	bool operator== (const EpollEvent& rhs) const {
		return (events == rhs.events) && (sptr_raw_metadata == rhs.sptr_raw_metadata);
	}

	bool operator!= (const EpollEvent& rhs) const {
		return (events != rhs.events) || (sptr_raw_metadata != rhs.sptr_raw_metadata);
	}

	void aaa() {
		printf("Using class integer\n");
	}
    };
}
#endif //LIBEPOLLPLUS_EPOLLEVENT_HPP
