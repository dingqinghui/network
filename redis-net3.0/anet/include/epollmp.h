
#ifndef __EPOLL_H__
#define __EPOLL_H__

#include "../include/error.h"

typedef struct fireEvent{
	int fd;
	int write;
	int read;
	int error
}fireEvent;

#define MP_ERR  -1
#define MP_OK  0

int  mpCreate(char* err,int maxEv);
void mpRelease();

int mpDel(char* err,int fd,int oldmask,int delmask);
int mpAdd(char* err,int fd, int oldmask,int addmask);

int mpWait(char* err,int w);
fireEvent* mpGetFireEvents();


#endif