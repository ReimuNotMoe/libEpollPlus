/*
    This file is part of libEpollPlus.
    Copyright (C) 2018 ReimuNotMoe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the MIT License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef LIBEPOLLPLUS_EPOLL_HPP
#define LIBEPOLLPLUS_EPOLL_HPP

#include "EpollEvent.hpp"

namespace EpollPlus {

    template<class T>
    class Epoll {
    private:
	mutable int fd_epoll = -1;
	std::unordered_map<void *, std::shared_ptr<RawMetaData>> ptr_keeper;

    public:
	Epoll() = default;

	explicit Epoll(bool cloexec) {
		Create(cloexec);
	}

	void Create(bool cloexec=false) {
		fd_epoll = epoll_create1(cloexec ? O_CLOEXEC : 0);
		if (fd_epoll < 0) {
			throw std::system_error(errno, std::system_category(), strerror(errno));
		}
	}

	void Add(const EpollEvent<T> &__ev) {
		if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, __ev.FD(), __ev.RawEvent().get())) {
			throw std::system_error(errno, std::system_category(), strerror(errno));
		}

		auto &sptr_userdata = __ev.RawUserData();
		ptr_keeper[sptr_userdata.get()] = sptr_userdata;
	}

	void Delete(const EpollEvent<T> &__ev) {
		auto &sptr_userdata = __ev.RawUserData();
		auto it = ptr_keeper.find(sptr_userdata.get());

		assert(it != ptr_keeper.end());
		ptr_keeper.erase(it);

		if (epoll_ctl(fd_epoll, EPOLL_CTL_DEL, __ev.FD(), nullptr)) {
			throw std::system_error(errno, std::system_category(), strerror(errno));
		}
	}

	void Modify(const EpollEvent<T> &__ev) {
		if (epoll_ctl(fd_epoll, EPOLL_CTL_MOD, __ev.FD(), __ev.RawEvent().get())) {
			throw std::system_error(errno, std::system_category(), strerror(errno));
		}
	}

	std::vector<EpollEvent<T>> Wait(int maxevents=64, int timeout=-1, const sigset_t *sigmask=nullptr) {
		std::vector<EpollEvent<T>> ret;

		epoll_event ep_events[maxevents];
//		auto sptr_events = std::shared_ptr<epoll_event>(ep_events, std::default_delete<epoll_event[]>());

		int rc = epoll_pwait(fd_epoll, ep_events, maxevents, timeout, sigmask);

		if (rc == -1)
			throw std::system_error(errno, std::system_category(), strerror(errno));

		for (size_t j=0; j<rc; j++) {
			auto &this_raw_event = ep_events[j];
			auto it_sptr_kept = ptr_keeper.find(this_raw_event.data.ptr);

			assert(it_sptr_kept != ptr_keeper.end());

			EpollEvent<T> this_event(this_raw_event.events, it_sptr_kept->second);
			ret.emplace_back(this_event);
		}

		return ret;
	}

    };
}

#endif //LIBEPOLLPLUS_EPOLL_HPP
