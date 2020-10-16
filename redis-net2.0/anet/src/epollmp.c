

#include<sys/epoll.h>
#include "../include/epollmp.h"
#include "../include/iomp.h"

typedef struct mpState
{
	int epfd;
	struct epoll_event* events;
	fireEvent* fevents;
	int maxEv;
} mpState;


static mpState* state = 0;


void mpCreate(int maxEv)
{
	state =  malloc(sizeof(mpState));
	if(!state)
		return -1;
	//Since Linux 2.6.8, the size argument is ignored, but must be greater than zero; see NOTES below.
	state->epfd = epoll_create(maxEv);
	if(state->epfd == -1){
		free(state);
		return -1;
	}

	state->events = malloc(sizeof(struct epoll_event) * maxEv);
	state->fevents = malloc(sizeof(fireEvent) * maxEv);
	state->maxEv = maxEv;
	return 0;
}

void mpRelease()
{
	free(state->events);
	free(state->fevents);
	close(state->epfd);
	free(state);
}

int mpAdd( int fd, int oldmask,int addmask)
{
	int mask = oldmask | addmask;

	struct epoll_event ee;
	ee.data.fd = fd;

	ee.events = 0;
 	if (mask & EV_READ) ee.events |= EPOLLIN;
    if (mask & EV_WRITE) ee.events |= EPOLLOUT;
    if (mask & EV_ERROR) ee.events |= EPOLLERR;

	if(oldmask == EV_NONE){
		return epoll_ctl(state->epfd, EPOLL_CTL_ADD, fd, &ee);
	}
	else{
		return epoll_ctl(state->epfd, EPOLL_CTL_MOD, fd, &ee);
	}
}



int mpDel(int fd,int oldmask,int delmask)
{
	struct epoll_event ee;
	ee.data.fd = fd;
	ee.events = 0;
	
	int mask = oldmask & (~delmask);
	if(mask == EV_NONE){
		return epoll_ctl(state->epfd, EPOLL_CTL_DEL, fd, 0);
	}
	else{
		if (mask & EV_READ) ee.events |= EPOLLIN;
		if (mask & EV_WRITE) ee.events |= EPOLLOUT;
		if (mask & EV_ERROR) ee.events |= EPOLLERR;

		return epoll_ctl(state->epfd, EPOLL_CTL_MOD, fd, &ee);
	}
}


int mpWait(int w)
{
	int cnt = epoll_wait(state->epfd, state->events, state->maxEv, w);

	for (size_t i = 0; i < cnt; i++)
	{
		int mask = 0;
		struct epoll_event* e= &(state->events[i]);

		if (e->events & EPOLLIN ) {
			 mask |= EV_READ;
		}

		if (e->events & EPOLLOUT ) {
			mask |= EV_WRITE;
		}

		if (e->events & EPOLLERR ){
			mask |=  EV_ERROR;
		} 

		state->fevents[i].mask = mask;
		state->fevents[i].fd = e->data.fd;

	}
	return cnt;
}

fireEvent* mpGetFireEvents(){
    return state->fevents;
}

