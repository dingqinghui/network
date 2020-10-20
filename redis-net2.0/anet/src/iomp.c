
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../include/epollmp.h"
#include "../include/iomp.h"

typedef struct evData{
    evCallBack* cb;
    void* udata;
}evData;


typedef struct fileEvent{
    int fd;
    int mask;
    evData rdata;
    evData wdata;
    evData edata;
}fileEvent;

typedef struct eventLoop
{
    int stop;
    fileEvent * events;      //all fd event    
    void* state;             //epoll
} eventLoop;

static eventLoop* loop = 0;

int evLoopCraete(int maxEv){

    loop = malloc(sizeof(eventLoop));
    if(!loop){
        return RT_ERROR;
    }
    loop->events = malloc(sizeof(fileEvent)*maxEv);
    for (size_t i = 0; i < maxEv; i++)
    {
       loop->events[i].mask = EV_NONE;
       loop->events[i].rdata.udata = 0;
       loop->events[i].wdata.udata = 0;
       loop->events[i].edata.udata = 0;
    }
 
    mpCreate(maxEv);

    return RT_OK;
}

void evLoopFree(){
    free(loop->events);
    free(loop);
}


int evLoopRegister(int fd,int mask,void* cb,void* udata){

    fileEvent* event = &(loop->events[fd]);

    if(-1 == mpAdd(fd,event->mask,mask) ){
        return RT_ERROR;
    }

    event->fd = fd;
    event->mask |= mask;

    evData* pdata = 0;
    if(mask & EV_READ)  pdata = &(event->rdata);
    if(mask & EV_WRITE) pdata = &(event->wdata);
    if(mask & EV_ERROR) pdata = &(event->edata);

    pdata->cb = cb;
    pdata->udata = udata;

    return RT_OK;
}

int evLoopUnregister(int fd,int mask){

    fileEvent* event = &(loop->events[fd]);

    if(-1 == mpDel(fd,event->mask,mask) ){
        return RT_ERROR;
    }

    event->mask &= ~mask;

    return RT_OK;
}

int onPollEvent(int mod){

    int w = mod == mod  == EV_WAIT_BLOCK ? -1 : FRAME_LOOP;

    int fireCnt = mpWait(w);
    if(fireCnt < 0){
        return RT_ERROR;
    }

    fireEvent* fevents = mpGetFireEvents();
    for (size_t i = 0; i < fireCnt; i++)
    {
        evData* pdata = 0;
        fireEvent* fevent = &(fevents[i]);
        int fd = fevent->fd;
        int mask = fevent->mask;

		fileEvent* event = &(loop->events[fd]);
       
        if(mask & EV_READ ){
            pdata = &(event->rdata);
        }
        if(mask & EV_WRITE  ){
           pdata = &(event->wdata);
        }
        if(mask & EV_ERROR ) {
            pdata = &(event->wdata);
        }
        if(pdata->cb){
            pdata->cb(fd,pdata->udata);
        }
    }
    return fireCnt;
}



int evLoopRun(int mod){
    loop->stop = 0;
    while(!loop->stop){
        onPollEvent( mod);
    }
}

void evLoopStop(){
    loop->stop = 1;
}
