

#include<sys/epoll.h>
#include <errno.h>
#include "../include/epollmp.h"
#include "../include/iomp.h"

#define mpSetError setError

typedef struct mpState
{
	int epfd;
	struct epoll_event* events;
	fireEvent* fevents;
	int maxEv;
} mpState;


static mpState* state = 0;


int  mpCreate(char*err,int maxEv)
{
	state =  malloc(sizeof(mpState));
	if(!state)
		return MP_ERR;
	//Since Linux 2.6.8, the size argument is ignored, but must be greater than zero; see NOTES below.
	state->epfd = epoll_create(maxEv);
	if(state->epfd == -1){
		free(state);
		mpSetError(err, "epoll_create: %s\n", strerror(errno));
		return MP_ERR;
	}

	state->events = malloc(sizeof(struct epoll_event) * maxEv);
	state->fevents = malloc(sizeof(fireEvent) * maxEv);
	state->maxEv = maxEv;
	return MP_OK;
}

void mpRelease()
{
	free(state->events);
	free(state->fevents);
	close(state->epfd);
	free(state);
}

int mpAdd( char* err,int fd, int oldmask,int addmask)
{
	int mask = oldmask | addmask;
	if(mask == oldmask){
		return 0;
	}
	
	struct epoll_event ee;
	ee.data.fd = fd;

	ee.events = 0;
 	if (mask & EV_READ) ee.events |= EPOLLIN;
    if (mask & EV_WRITE) ee.events |= EPOLLOUT;
    if (mask & EV_ERROR) ee.events |= EPOLLERR;

	if(oldmask == EV_NONE){
		if( epoll_ctl(state->epfd, EPOLL_CTL_ADD, fd, &ee) < 0 ){
			mpSetError(err, "mpAdd(epoll_ctl-EPOLL_CTL_ADD): %s\n", strerror(errno));
			return MP_ERR;
		}
	}
	else{
		if( epoll_ctl(state->epfd, EPOLL_CTL_MOD, fd, &ee) < 0 ){
			mpSetError(err, "mpAdd(epoll_ctl-EPOLL_CTL_MOD): %s\n", strerror(errno));
			return MP_ERR;
		}
	}	
	return MP_OK;
}



int mpDel(char* err,int fd,int oldmask,int delmask)
{
	struct epoll_event ee;
	ee.data.fd = fd;
	ee.events = 0;
	
	int mask = oldmask & (~delmask);
	if(mask == oldmask){
		return 0;
	}
	if(mask == EV_NONE){
		if( epoll_ctl(state->epfd, EPOLL_CTL_DEL, fd, 0) < 0){
			mpSetError(err, "mpDel(epoll_ctl-EPOLL_CTL_DEL): %s\n", strerror(errno));
			return MP_ERR;
		}
	}
	else{
		if (mask & EV_READ) ee.events |= EPOLLIN;
		if (mask & EV_WRITE) ee.events |= EPOLLOUT;
		if (mask & EV_ERROR) ee.events |= EPOLLERR;
		if( epoll_ctl(state->epfd, EPOLL_CTL_MOD, fd, &ee) < 0) {
			mpSetError(err, "mpDel(epoll_ctl-EPOLL_CTL_MOD): %s\n", strerror(errno));
			return MP_ERR;
		}
	}
	return MP_OK;
}


int mpWait(char* err,int w)
{
	int cnt = epoll_wait(state->epfd, state->events, state->maxEv, w);
	if(cnt < 0){
		mpSetError(err, "mpWait(epoll_wait): %s\n", strerror(errno));
		return MP_ERR;
	}

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

