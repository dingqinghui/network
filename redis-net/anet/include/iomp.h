
#ifndef __MP_H__
#define __MP_H__



#define EV_NONE     0
#define EV_WRITE    1
#define EV_READ     1<<1
#define EV_ERROR    1<<2


#define EV_WAIT_BLOCK 1
#define EV_WAIT_FPS  2

#define FRAME_LOOP   10

typedef void FileCb(int);

typedef struct FileEvent{
    int fd;
    int mask;
    FileCb* readCb;
    FileCb* writeCb;
    FileCb* expectCb;
}FileEvent;


typedef struct EventLoop
{
    int stop;
    FileEvent * events;  //all fd event    
    void* state;             //epoll
} EventLoop;


EventLoop* createEventLoop();
void freeEventLoop(EventLoop* loop);
int addEvent(EventLoop* loop,int fd,int mask,void* cb);
int delEvent(EventLoop* loop,int fd,int mask);
int waitEvent(EventLoop* loop,int mod);
int runEventLoop(EventLoop* loop,int mod);
void stopEventLoop(EventLoop* loop);


#endif