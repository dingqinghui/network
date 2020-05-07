
#include<sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/socket_epoll.h"


int sp_create()
{
	return epoll_create(MAX_FD);
}

int sp_release(int efd)
{
	close(efd);
}

bool  sp_invaild(int efd)
{
	return efd != -1;
}

int sp_add(int efd, int socket, void* ud)
{
	struct epoll_event e;
	e.events = EPOLLIN;
	e.data.ptr = ud;
	return epoll_ctl(efd, EPOLL_CTL_ADD, socket, &e);
}

int sp_del(int efd, int socket)
{
	return epoll_ctl(efd, EPOLL_CTL_DEL, socket, NULL);
}

int sp_write(int efd, int socket, void* ud, bool bwrite)
{
	struct epoll_event e;
	e.events = EPOLLIN | (bwrite ? EPOLLOUT : 0);
	e.data.ptr = ud;

	return epoll_ctl(efd, EPOLL_CTL_MOD, socket, &e);
}

int sp_wait(int epfd, struct event* e, const int max)
{
	struct epoll_event events[max];
	int cnt = epoll_wait(epfd, events, max, 0);

	for (size_t i = 0; i < cnt; i++)
	{
		e[i].udata = events[i].data.ptr;
		e[i].read = (events[i].events & EPOLLIN) != 0;
		e[i].write = (events[i].events & EPOLLOUT) != 0;
		e[i].error = (events[i].events & EPOLLERR | events[i].events & EPOLLHUP) != 0;
	}
	return cnt;
}



