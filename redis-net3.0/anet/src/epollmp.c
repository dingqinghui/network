

#include<sys/epoll.h>
#include <errno.h>
#include "../include/epollmp.h"
#include "../include/iomp.h"
#include "../include/zmemory.h"

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
	state =  zmalloc(sizeof(mpState));

	//Since Linux 2.6.8, the size argument is ignored, but must be greater than zero; see NOTES below.
	state->epfd = epoll_create(maxEv);
	if(state->epfd == -1){
		mpSetError(err, "epoll_create: %s\n", strerror(errno));
		return NET_RET_ERROR;
	}
	state->events = zmalloc(sizeof(struct epoll_event) * maxEv);
	state->fevents = zmalloc(sizeof(fireEvent) * maxEv);
	state->maxEv = maxEv;
	return NET_RET_OK;
}

void mpRelease()
{
	CHECK_PTR_ERR(state)
	zfree(state->events);
	zfree(state->fevents);
	close(state->epfd);
	zfree(state);
	state = 0;
}

// 客户端异常关闭，并不会通知服务器。正常关闭时read到0后，异常断开时检测不到的。服务器再给一个已经关闭
// 的socket写数据时，会出错，这时候，服务器才明白对方可能已经异常断开了（读也
// 可以）。
// Epoll中就是向已经断开的socket写或者读，会发生EPollErr，即表明已经断开。

int mpAdd( char* err,int fd, int oldmask,int addmask)
{
	CHECK_PTR_ERR(state)

	int mask = oldmask | addmask;
	if(mask == oldmask){
		return NET_RET_OK;
	}

	struct epoll_event ee;
	ee.data.fd = fd;

	ee.events = 0;
 	if (mask & EV_MASK_READ) ee.events |= EPOLLIN;	 //对端正常关闭 触发 read == 0
    if (mask & EV_MASK_WRITE) ee.events |= EPOLLOUT;
    if (mask & EV_MASK_ERROR) ee.events |= EPOLLERR;  //对端异常关闭 本端进行读写时触发

	if(oldmask == EV_MASK_NONE){
		if( epoll_ctl(state->epfd, EPOLL_CTL_ADD, fd, &ee) < 0 ){
			mpSetError(err, "mpAdd(epoll_ctl-EPOLL_CTL_ADD): %s\n", strerror(errno));
			return NET_RET_ERROR;
		}
	}
	else{
		if( epoll_ctl(state->epfd, EPOLL_CTL_MOD, fd, &ee) < 0 ){
			mpSetError(err, "mpAdd(epoll_ctl-EPOLL_CTL_MOD): %s\n", strerror(errno));
			return NET_RET_ERROR;
		}
	}	
	return NET_RET_OK;
}



int mpDel(char* err,int fd,int oldmask,int delmask)
{
	CHECK_PTR_ERR(state)

	struct epoll_event ee;
	ee.data.fd = fd;
	ee.events = 0;

	int mask = oldmask & (~delmask);
	if(mask == oldmask){
		return 0;
	}
	if(mask == EV_MASK_NONE){
		if( epoll_ctl(state->epfd, EPOLL_CTL_DEL, fd, 0) < 0){
			mpSetError(err, "mpDel(epoll_ctl-EPOLL_CTL_DEL): %s\n", strerror(errno));
			return NET_RET_ERROR;
		}
	}
	else{
		if (mask & EV_MASK_READ) ee.events |= EPOLLIN;
		if (mask & EV_MASK_WRITE) ee.events |= EPOLLOUT;
		if (mask & EV_MASK_ERROR) ee.events |= EPOLLERR;
		if( epoll_ctl(state->epfd, EPOLL_CTL_MOD, fd, &ee) < 0) {
			mpSetError(err, "mpDel(epoll_ctl-EPOLL_CTL_MOD): %s\n", strerror(errno));
			return NET_RET_ERROR;
		}
	}
	return NET_RET_OK;
}



int mpWait(char* err,int w)
{
	CHECK_PTR_ERR(state)

	int cnt = epoll_wait(state->epfd, state->events, state->maxEv, w);

	if(cnt < 0){
		mpSetError(err, "mpWait(epoll_wait): %s\n", strerror(errno));
		return NET_RET_ERROR;
	}
	for (size_t i = 0; i < cnt; i++)
	{
		struct epoll_event* e= &(state->events[i]);
		state->fevents[i].read = e->events & EPOLLIN ? 1 : 0;
		state->fevents[i].write = e->events & EPOLLOUT ? 1 : 0;
		state->fevents[i].error = e->events & EPOLLERR? 1 : 0;
		state->fevents[i].fd = e->data.fd;

		int mask = 0;
		if(e->events & EPOLLIN )   mask |=  EV_MASK_READ;
		if(e->events & EPOLLOUT )  mask |=  EV_MASK_WRITE;
		if(e->events & EPOLLERR )  mask |=  EV_MASK_ERROR;
	}

	return cnt;
}

fireEvent* mpGetFireEvents(){
	CHECK_PTR_ERR(state)
    return state->fevents;
}

