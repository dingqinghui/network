#ifndef __server_h__
#define __server_h__

#include "socket_epoll.h"
#include "socket.h"


typedef struct server
{
	int epfd;
	int maxfd;
	struct event* events;
	int event_n;
	int event_index;

	struct st_socket** socket_slot;

} server;

static struct server* s = NULL;

server* server_create(int maxfd);
void server_release();
int server_reverse_id();
int server_add_socket(st_socket* so);
int server_del_socket(id);
int server_lisent(char* IP, int port, void* callback);
int server_connect(char* IP, int port, void* callback);
int  server_send(int id, char* buffer, int size);
void on_network();
int on_server_epoll();
int on_report_error(st_socket* so);
int on_report_connect(st_socket* so);	
int on_report_accpet(st_socket* l_so);
int on_report_read(st_socket* so);
int server_register_event(int id, int event_type, void* callback);
void on_work();
void server_run();


#endif