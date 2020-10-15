
#ifndef __EPOLL_H__
#define __EPOLL_H__

#include<sys/epoll.h>

typedef struct fireEvent{
	int fd;
	int mask;
}fireEvent;

typedef struct mpState
{
	int epfd;
	struct epoll_event* events;
	fireEvent* fevents;
	int maxEv;
} mpState;

mpState* mpCreate();
void mpRelease(mpState* state);
int mpDel(mpState* state, int fd,int oldmask,int delmask);
int mpAdd(mpState* state, int fd, int oldmask,int addmask);
int mpWait(mpState* state,int w);


#endif