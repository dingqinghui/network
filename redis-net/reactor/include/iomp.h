
#ifndef __MP_H__
#define __MP_H__

#define RT_ERROR  -1
#define RT_OK  0


#define EV_NONE     0
#define EV_WRITE    1
#define EV_READ     1<<1
#define EV_ERROR    1<<2


#define EV_WAIT_BLOCK 1
#define EV_WAIT_FPS  2


#define FRAME_LOOP   10


typedef void evCallBack(int);


int createEventLoop(int maxEv);
void freeEventLoop();

int registerEvent(int fd,int mask,void* cb);
int removeEvent(int fd,int mask);

int runEventLoop(int mod);
void stopEventLoop();


#endif