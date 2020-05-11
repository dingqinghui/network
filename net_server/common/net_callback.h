#ifndef  __net_callback__h__
#define  __net_callback__h__

#include "connection_mgr.h"
#include "dispatch.h"

int Recv(int id, char* buffer, int size) {
	st_connection* con = get_connection(id);
	if (!con) {
		printf("on_recv  id:%d   buffer:%s con not exist ........\n", id, buffer);
		return -1;
	}

	printf("on_recv  id:%d   buffer:%s ........\n", id, buffer);

	server_mgr_send(id, buffer, size);

	connection_write(con, buffer, size);

	Dispatch(con);
}


int Connect(int id) {

	printf("on_connect  id:%d ........\n", id);

	server_mgr_send(id, "123456", 7);

	st_connection* con = create_connect(id);
	add_connection(con);

}

int Accept(int id) {

	printf("on_accpet  id:%d ........\n", id);

	st_connection* con = create_connect(id);
	add_connection(con);
}

int Close(int id) {

}

int Error(int id) {

}


int TimeOut() {
	h_t_mgr_tick();
}

#endif