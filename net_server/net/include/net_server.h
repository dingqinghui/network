#ifndef __server_h__
#define __server_h__

#include "socket_epoll.h"
#include "socket.h"

typedef int (*accpet_fun)(int id);
typedef int (*connect_fun)(int id);
typedef int (*close_fun)(int id);
typedef int (*recv_fun)(int id, void* buffer, int size);
typedef int (*error_fun)(int id);
typedef int (*timeout_fun)();

typedef struct net_server
{
	int epfd;
	int maxfd;
	struct event* events;
	int event_n;
	int event_index;
	struct st_socket** socket_slot;
	int pipes[2];

	void* connect_callback;
	void* accept_callback;
	void* recv_callback;
	void* close_callback;
	void* error_callback;
	void* timeout_callback;

} net_server;

static struct net_server* s = NULL;

net_server* net_server_create(int maxfd, void* connect_callback, void* accept_callback, void* recv_callback, void* close_callback, void* error_callback, void* timeout_callback;);
void    net_server_release();

int  net_server_lisent(char* IP, int port);
int  net_server_connect(char* IP, int port);
int  net_server_send(int id, char* buffer, int size);
int  net_server_epoll(int timeout);

#endif