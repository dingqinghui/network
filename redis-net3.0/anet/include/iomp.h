
#ifndef __MP_H__
#define __MP_H__


#define EV_MASK_NONE     0
#define EV_MASK_WRITE    1
#define EV_MASK_READ     1<<1
#define EV_MASK_ERROR    1<<2


#define EV_WAIT_BLOCK 1
#define EV_WAIT_FPS  2


#define FRAME_LOOP   10


typedef void evCallBack(int fd,void* udata);

int  evLoopCraete(int maxEv);
void evLoopFree();

int  evLoopRegister(int fd,int mask,void* cb,void* udata);
int  evLoopUnregister(int fd,int mask);
int  evLoopRemove(int fd);

int  evLoopRun(int mod);
void evLoopStop();


#endif