
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "../include/zmemory.h"
#include "../include/epollmp.h"
#include "../include/iomp.h"

#define EV_MASK_ALL  (EV_MASK_READ | EV_MASK_WRITE | EV_MASK_ERROR)

#define EV_MASK_MAX      ( EV_MASK_ERROR + 1 )

#define LOOP  loop


#define CHECK_FD_VALUE(fd)  \
if(LOOP->maxev <= fd)\
{ \
    PRINT_ERR("fd is over loop size")\
    return NET_RET_ERROR; \
} 

#define CHECK_MASK_INVAILD(mask) \
if(mask != EV_MASK_READ &&  mask != EV_MASK_WRITE \
    &&  mask != EV_MASK_ERROR ) \
{\
    PRINT_ERR("invaild mask")\
    return NET_RET_ERROR;\
}


typedef struct evData{
    evCallBack* cb;
    void* udata;
}evData;


typedef struct fileEvent{
    int fd;
    int mask;
    evData evlist[EV_MASK_MAX];
}fileEvent;

typedef struct eventLoop
{
    fileEvent * events;      //all fd event    
    int maxev;
} eventLoop;



static eventLoop* loop = 0;

int evLoopCraete(int maxEv){

    LOOP = (eventLoop*)zmalloc(sizeof(eventLoop));
   
    LOOP->events = (fileEvent*)zmalloc( sizeof(fileEvent) * maxEv );
    printf("loop->events:%p\n",LOOP->events);
    
    char err[NET_ERR_LEN];
    if( mpCreate(err,maxEv) == NET_RET_ERROR){
        PRINT_ERR(err);
        return NET_RET_ERROR;
    }

    for (size_t i = 0; i < maxEv; ++i)
    {
        LOOP->events[i].fd = -1;
        LOOP->events[i].mask = EV_MASK_NONE;
        for (size_t j = 0; j < EV_MASK_MAX; ++j)
        {
            (LOOP->events)[i].evlist[j].udata = 0;
            (LOOP->events)[i].evlist[j].cb = 0;
        }
    }
    LOOP->maxev = maxEv;

	signal(SIGPIPE, SIG_IGN);
    return NET_RET_OK;
}



void evLoopFree(){
    CHECK_PTR_ERR(LOOP)
    zfree(LOOP->events);
    mpRelease();
    zfree(LOOP);
    LOOP = 0;
}


int evLoopRegister(int fd,int mask,void* cb,void* udata){
    CHECK_PTR_ERR(LOOP)
    CHECK_PTR_ERR(cb)
    CHECK_PTR_ERR(udata)
    CHECK_FD_VALUE(fd)
    CHECK_MASK_INVAILD(mask)

    if(mask == EV_MASK_NONE){
        return NET_RET_OK;
    }
  
    fileEvent* event = &(LOOP->events[fd]);
    char err[NET_ERR_LEN];
    if(NET_RET_ERROR == mpAdd(err,fd,event->mask,mask) ){
        PRINT_ERR(err);
        return NET_RET_ERROR;
    }

    event->fd = fd;
    event->mask |= mask;
    event->evlist[mask].cb = cb;
    event->evlist[mask].udata= udata;
    return NET_RET_OK;
}

int evLoopUnregister(int fd,int mask){
    CHECK_PTR_ERR(LOOP)
    CHECK_FD_VALUE(fd)
    CHECK_MASK_INVAILD(mask)

    fileEvent* event = &(LOOP->events[fd]);

    char err[NET_ERR_LEN];
    if(NET_RET_ERROR == mpDel(err,fd,event->mask,mask) ){
        PRINT_ERR(err);
        return NET_RET_ERROR;
    }
    event->mask &= ~mask;
    event->evlist[mask].cb = 0;
    event->evlist[mask].udata = 0;
    
    if(event->mask == EV_MASK_NONE){
        event->fd = 0;  
    }
    return NET_RET_OK;
}

int evLoopRemove(int fd){
    CHECK_PTR_ERR(LOOP)
    CHECK_FD_VALUE(fd)

    char err[NET_ERR_LEN];
    fileEvent* event = &(LOOP->events[fd]);
    if(NET_RET_ERROR == mpDel(err,fd,event->mask,EV_MASK_ALL) ){
        PRINT_ERR(err);
        return NET_RET_ERROR;
    }
    event->fd = -1;
    event->mask = EV_MASK_NONE;
    return NET_RET_OK;
}


int onPollEvent(){
    CHECK_PTR_ERR(LOOP)

    char err[NET_ERR_LEN];
    int fireCnt = mpWait(err,FRAME_LOOP);
    if(fireCnt == NET_RET_ERROR){
        PRINT_ERR(err);
        return NET_RET_ERROR;
    }

    fireEvent* fevents = mpGetFireEvents();
    for (size_t i = 0; i < fireCnt; i++)
    {
        evData* pdata = 0;
        fireEvent* fevent = &(fevents[i]);
        int fd = fevent->fd;

		fileEvent* event = &(LOOP->events[fd]);

        if(fevent->read){
            pdata = &(event->evlist[EV_MASK_READ]);
            if(pdata->cb && event->fd > 0) pdata->cb(fd,pdata->udata);
        }
        if(fevent->write){
           pdata = &(event->evlist[EV_MASK_WRITE]);
           if(pdata->cb && event->fd > 0) pdata->cb(fd,pdata->udata);
        }
        if(fevent->error) {
            pdata = &(event->evlist[EV_MASK_ERROR]);
            if(pdata->cb && event->fd > 0) pdata->cb(fd,pdata->udata);
        }
        
    }
    return fireCnt;
}


int evLoopPoll(){
   return onPollEvent();
}

