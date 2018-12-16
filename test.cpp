#include "EpollPlus.hpp"

using namespace EpollPlus;

int main() {
	std::string mystr = "aaaaaa";

	Epoll<std::string> myep(true);

	char buf[32768];


	EpollEvent<std::string> ev0(STDIN_FILENO, &mystr, EPOLLIN);


	myep.Add(ev0.SetFD(0).SetUserData(&mystr));

	int i=10;

	int flag = fcntl(0, F_GETFL) | O_NONBLOCK;
	fcntl(0, F_SETFL, flag);


	EpollEvent<std::string> thisep0;

	while (i--) {
		for (auto &ev: myep.Wait()) {
			if (ev & EPOLLIN)
			printf("fd=%d, events=%d, data=%p %s\n", ev.FD(), ev.Events(), ev.UserData(),
			       ev.UserData()->c_str());

			thisep0 = ev;
			read(STDIN_FILENO, buf, 32768);
		}
	}


	ev0 |= EPOLLOUT;
	myep.Modify(ev0);
	myep.Delete(thisep0);

	return 0;
}