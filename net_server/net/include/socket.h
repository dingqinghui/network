#ifndef __socket_h__
#define __socket_h__

#include <stdbool.h>
#include "socket_event.h"

#define SOCKET_STAT_LISENT			1
#define SOCKET_STAT_ESTABLISHED		2
#define SOCKET_STAT_CLOSE			3
#define SOCKET_STAT_CONNECTING	    4    

struct st_callback;
struct event_param;

typedef struct st_socket
{
	int fd;
	int stat;
	int id;
}st_socket;


char* socket_print(st_socket* so);
int  socket_set_noblock(int fd);
st_socket* socket_create();
void socket_release(st_socket* so);
bool socket_is_close(st_socket* so);
int socket_close(st_socket* so);
bool socket_is_lisent(st_socket* so);
bool socket_is_esta(st_socket* so);
bool socket_is_connecting(st_socket* so);
void socket_set_stat(st_socket* so, int stat);
void socket_get_stat(st_socket* so);
void socket_set_fd(st_socket* so, int fd);
void socket_set_id(st_socket* so, int id);
st_socket* socket_lisent(char* IP, int port);
st_socket* socket_connect(char* IP, int port);
st_socket* socket_accept(st_socket* l_socket);
int socket_read(st_socket* so, char* buffer, int size);
int socket_write(st_socket* so, char* buffer, int size);
int socket_register_event(st_socket* so, int event_type, void* con_call);
int socket_emit_event(st_socket* so, int event_type, struct event_param* param);


#endif