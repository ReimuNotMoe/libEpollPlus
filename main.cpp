#include "EpollPlus.hpp"

using namespace EpollPlus;

int main() {
	std::string mystr = "aaaaaa";

	Epoll<std::string> myep(true);

	char buf[32768];
	EpollEvent<std::string> ev0(STDIN_FILENO, &mystr, EPOLLIN);

	myep.Add(ev0);

	int i=10;

	int flag = fcntl(0, F_GETFL) | O_NONBLOCK;
	fcntl(0, F_SETFL, flag);

	while (1) {
		for (auto &ev: myep.Wait()) {
//			if (ev & EPOLLIN)
//			printf("fd=%d, events=%d, data=%p %s\n", ev.FD(), ev.Events(), ev.UserData(),
//			       ev.UserData()->c_str());

			read(STDIN_FILENO, buf, 32768);
		}
	}

	return 0;
}