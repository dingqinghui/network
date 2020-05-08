#ifndef __socket_event__
#define __socket_event__


#include "socket.h"


#define SOCKET_EVENT_CONNECTED   1 
#define SOCKET_EVENT_ACCPET		 2 
#define SOCKET_EVENT_RECV		 3 
#define SOCKET_EVENT_CLOSE       4 
#define SOCKET_EVENT_ERROR       5 
#define SOCKET_EVENT_MAX         5 

typedef int (*accpet_fun)(int id);
typedef int (*connect_fun)(int id);
typedef int (*close_fun)(int id);
typedef int (*recv_fun)(int id, void* buffer, int size);
typedef int (*error_fun)(int id);




typedef struct event_param{
	void* ud;
	int size;
}event_param;


typedef struct st_callback
{
	struct st_socket* so;
	void* callback_list[SOCKET_EVENT_MAX];
}st_callback;


int register_event(st_callback* sc, int event_type, void* con_call);
int emit_event(st_callback* sc, int event_type, event_param* param);


#endif