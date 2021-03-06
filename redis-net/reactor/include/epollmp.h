
#ifndef __EPOLL_H__
#define __EPOLL_H__



typedef struct fireEvent{
	int fd;
	int mask;
}fireEvent;



void* mpCreate();
void mpRelease();

int mpDel(int fd,int oldmask,int delmask);
int mpAdd(int fd, int oldmask,int addmask);

int mpWait(int w);
fireEvent* getFireEvents();


#endif