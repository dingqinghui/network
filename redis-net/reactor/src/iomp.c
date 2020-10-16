
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "../include/epollmp.h"

typedef struct fileEvent{
    int fd;
    int mask;
    evCallBack* readCb;
    evCallBack* writeCb;
    evCallBack* expectCb;
}fileEvent;




typedef struct eventLoop
{
    int stop;
    fileEvent * events;  //all fd event    
    void* state;             //epoll
} eventLoop;

static eventLoop* loop = 0;

int createEventLoop(int maxEv){

    loop = malloc(sizeof(EventLoop));
    if(!loop){
        return RT_ERROR;
    }
    loop->events = malloc(sizeof(fileEvent)*maxEv);
    for (size_t i = 0; i < maxEv; i++)
    {
       loop->events[i].mask = EV_NONE;
    }
 
    return RT_OK;
}

void freeEventLoop(){
    free(loop->events);
    free(loop);
}


int registerEvent(int fd,int mask,void* cb){

    fileEvent* event = &(loop->events[fd]);

    if(-1 == mpAdd(fd,event->mask,mask) ){
        return RT_ERROR;
    }

    event->fd = fd;
    event->mask |= mask;
    if(mask & EV_READ)  event->readCb = cb;
    if(mask & EV_WRITE) event->writeCb = cb;
    if(mask & EV_ERROR) event->expectCb = cb;

    return RT_OK;
}

int removeEvent(int fd,int mask){

    FileEvent* event = &(loop->events[fd]);

    if(-1 == mpDel(fd,event->mask,mask) ){
        return RT_ERROR;
    }

    event->mask &= ~mask;

    return RT_OK;
}

int onPollEvent(int mod){

    int w = mod == mod  == EV_WAIT_BLOCK ? -1 : 10;

    int fireCnt = mpWait(w);
    if(fireCnt < 0){
        return RT_ERROR;
    }

     fireEvent* fevents = mpGetFireEvents()
    for (size_t i = 0; i < fireCnt; i++)
    {
        fireEvent* fevent = &(fevents[i]);
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
        onPollEvent(loop, mod);
    }
}

void stopEventLoop(EventLoop* loop){
    loop->stop = 1;
}
