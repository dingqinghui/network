#ifndef socke_epoll
#define socke_epoll


#include <stdbool.h>

struct event
{
    void* udata ;
    bool  write;
    bool  read;
    bool  error;
} event;

static const int MAX_FD = 1024;

int sp_create();
int sp_release(int efd);
bool sp_invaild(int efd);
int sp_add(int efd, int socket, void* ud);
int sp_del(int efd, int socket);
int sp_write(int efd, int socket, void* ud, bool bwrite);
int sp_wait(int epfd, struct event* e, const int max,int timeout);


#endif