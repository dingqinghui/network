#include <stdio.h>
#include "../net/include/server.h"




int g_id = 0;

int on_recv(int id, char* buffer, int size) {

	printf("on_recv  id:%d   buffer:%s ........\n", id, buffer);

	server_send(g_id, buffer, size);
}

int on_connect(int id) {

	printf("on_connect  id:%d ........\n", id);
	g_id = id;

	server_send(g_id, "123456", 7);

	server_register_event(id, SOCKET_EVENT_RECV, on_recv);
}



int main()
{
	  server_create(1024);


	  g_id = server_connect("127.0.0.1", 8888, on_connect);

	  server_run();

	  printf("退出进程");

	  server_release();


    return 0;
}