//
// Created by root on 18-12-5.
//

#ifndef LIBEPOLLPLUS_EPOLLEVENT_HPP
#define LIBEPOLLPLUS_EPOLLEVENT_HPP

#include "CommonIncludes.hpp"

namespace EpollPlus {

    template<class T, typename = void>
    class EpollEvent {
    private:
	int fd = -1;
	epoll_event *raw_event = nullptr;
	bool is_existing_raw_metadata = false;
	std::shared_ptr<epoll_event> sptr_raw_events_array;
	std::shared_ptr<RawMetaData> sptr_raw_metadata;
	size_t pos_rawevents = 0;

    public:
	EpollEvent() = default;

//	explicit EpollEvent(const epoll_event &__event) {
//		raw_event = new epoll_event;
//		memcpy(raw_event, &__event, sizeof(epoll_event));
//	}

	EpollEvent(const std::shared_ptr<epoll_event> &__sptr_rawevents_array, const std::shared_ptr<RawMetaData> &__sptr_rawmetadata, size_t __pos) {
		is_existing_raw_metadata = true;
		sptr_raw_events_array = __sptr_rawevents_array;
		sptr_raw_metadata = __sptr_rawmetadata;
		pos_rawevents = __pos;
		raw_event = sptr_raw_events_array.get() + sizeof(epoll_event) * __pos;
		fd = (*sptr_raw_metadata).fd;
	}

	EpollEvent(int user_fd, T *user_data, uint32_t events) {
		fd = user_fd;
		raw_event = new epoll_event();

		sptr_raw_metadata = std::make_shared<RawMetaData>();
		auto raw_metadata = sptr_raw_metadata.get();

		raw_metadata->fd = fd;
		raw_metadata->data.ptr = user_data;
		raw_event->data.ptr = (void *)raw_metadata;

		raw_event->events = events;
	}

	~EpollEvent() {
		if (raw_event) {
			if (!is_existing_raw_metadata)
				delete raw_event;
		}
	}

	T* const UserData() const {
		return reinterpret_cast<T *>((*sptr_raw_metadata).data.ptr);
	}

	void UserData(T *ptr) {
		(*sptr_raw_metadata).data.ptr = ptr;
	}

	int FD() const {
		return fd;
	}

	const std::shared_ptr<RawMetaData> &RawUserData () const {
		return sptr_raw_metadata;
	}

	epoll_event *RawEvent() const {
		return raw_event;
	};

	const uint32_t Events() {
		return raw_event->events;
	}

	uint32_t operator& (uint32_t rhs) {
		return raw_event->events & rhs;
	}

	uint32_t operator| (uint32_t rhs) {
		return raw_event->events | rhs;
	}

	uint32_t operator&= (uint32_t rhs) {
		return (raw_event->events &= rhs);
	}

	uint32_t operator|= (uint32_t rhs) {
		return (raw_event->events |= rhs);
	}

	bool operator== (const EpollEvent& rhs) {
		return (raw_event->events == rhs.raw_event->events) && (raw_event->data.ptr == rhs.raw_event->data.ptr);
	}

	bool operator!= (const EpollEvent& rhs) {
		return (raw_event->events != rhs.raw_event->events) || (raw_event->data.ptr != rhs.raw_event->data.ptr);
	}

	void aaa() {
		printf("Using class T\n");
	}
    };


//    template<class T>
//    class EpollEvent<T, typename std::enable_if<is_int<T>::value>::type> {
//    private:
//	epoll_event *raw_event = nullptr;
//	bool is_existing_raw_metadata = false;
//	std::shared_ptr<epoll_event *> sptr_raw_events_array;
//	size_t pos_rawevents = 0;
//    public:
//	EpollEvent() = default;
//
//	explicit EpollEvent(const epoll_event &__event) {
//		raw_event = new epoll_event;
//	}
//
//	EpollEvent(const std::shared_ptr<epoll_event *> &__events_sptr, size_t __pos) {
//		sptr_raw_events_array = __events_sptr;
//		pos_rawevents = __pos;
//		raw_event = *sptr_raw_events_array + sizeof(epoll_event) * __pos;
//	}
//
//	EpollEvent(T user_data, uint32_t events) {
//		raw_event = new epoll_event;
//		memcpy(&raw_event->data.u64, &user_data, sizeof(T));
//		raw_event->events = events;
//	}
//
//	~EpollEvent() {
//		if (raw_event) {
//			if (!is_existing_raw_metadata)
//				delete raw_event;
//		}
//	}
//
//	void aaa() {
//		printf("Using class ints\n");
//	}
//
//
//    };
}
#endif //LIBEPOLLPLUS_EPOLLEVENT_HPP
