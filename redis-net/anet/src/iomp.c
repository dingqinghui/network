
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../include/iomp.h"
#include "../include/epollmp.h"




EventLoop* createEventLoop(int maxEv){

    EventLoop* loop = (EventLoop*)malloc(sizeof(EventLoop));
    if(!loop){
        return NULL;
    }

    loop->state = mpCreate(maxEv);
    if(!loop->state){
         free(loop);
         return NULL;
    }
    loop->events = malloc(sizeof(FileEvent)*maxEv);

    for (size_t i = 0; i < maxEv; i++)
    {
       loop->events[i].mask = EV_NONE;
    }
 
    return loop;
}

void freeEventLoop(EventLoop* loop){
	mpRelease(loop->state);
    free(loop->events);
    free(loop);
}


int addEvent(EventLoop* loop,int fd,int mask,void* cb){

    FileEvent* event = &(loop->events[fd]);

    if(-1 == mpAdd(loop->state, fd,event->mask,mask) ){
        return -1;
    }

    event->fd = fd;
    event->mask |= mask;
    if(mask & EV_READ)  event->readCb = cb;
    if(mask & EV_WRITE) event->writeCb = cb;
    if(mask & EV_ERROR) event->expectCb = cb;

    printf("addEvent fd:%d mask:%d\n",fd,mask);
    return 0;
}

int delEvent(EventLoop* loop,int fd,int mask){

    FileEvent* event = &(loop->events[fd]);

    if(-1 == mpDel(loop->state,fd,event->mask,mask) ){
        return -1;
    }

    event->mask &= ~mask;


    return 0;
}

int waitEvent(EventLoop* loop,int mod){

    int w = mod == mod  == EV_WAIT_BLOCK ? -1 : 10;

    int fireCnt = mpWait(loop->state,w);
    if(fireCnt <= 0){
        return -1;
    }

    for (size_t i = 0; i < fireCnt; i++)
    {
        fireEvent* fevent = &(((mpState*)(loop->state))->fevents[i]);
        int fd = fevent->fd;
        int mask = fevent->mask;

		FileEvent* event = &(loop->events[fd]);

        if(mask & EV_READ && event->readCb){
            event->readCb(fd);
        }
        if(mask & EV_WRITE && event->writeCb ){
           
            event->writeCb(fd);
        }
        if(mask & EV_ERROR && event->expectCb) {
            event->expectCb(fd);
        }
    }
    return fireCnt;
}

int runEventLoop(EventLoop* loop,int mod){
    loop->stop = 0;
    while(!loop->stop){
        waitEvent(loop, mod);
    }
}

void stopEventLoop(EventLoop* loop){
    loop->stop = 1;
}
