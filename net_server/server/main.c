#include <stdio.h>
#include "../net/include/server.h"

int g_id = 0;


int on_recv(int id, char* buffer, int size) {

	printf("on_recv  id:%d    buffer:%s ........\n", id,  buffer);

	server_send(g_id, buffer, size);
}

int on_accpet(int id) {

	printf("on_accpet  id:%d ........\n", id);
	g_id = id;

	server_register_event(g_id, SOCKET_EVENT_RECV,on_recv);
}



int main()
{
    server_create(1024);



	server_lisent("127.0.0.1", 8888, on_accpet);

	server_run();
    printf("退出进程");

    server_release();
    return 0;
}