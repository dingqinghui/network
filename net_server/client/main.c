#include <stdio.h>
//#include "../net/include/server.h"
#include "../common/buffer.h"



//int g_id = 0;
//
//int on_recv(int id, char* buffer, int size) {
//
//	printf("on_recv  id:%d   buffer:%s ........\n", id, buffer);
//
//	server_send(g_id, buffer, size);
//}
//
//int on_connect(int id) {
//
//	printf("on_connect  id:%d ........\n", id);
//	g_id = id;
//
//	server_send(g_id, "123456", 7);
//
//	server_register_event(id, SOCKET_EVENT_RECV, on_recv);
//}



int main()
{
	/*  server_create(1024);


	  g_id = server_connect("127.0.0.1", 8888, on_connect);

	  server_run();

	  printf("退出进程");

	  server_release();*/


	st_buffer* buffer = create_buffer();
	for (int index = 1; index < 100; index++)
	{
		int nwrite = write_to_buffer(buffer, "123456", 6);
		printf("write_to_buffer size :%d \n", nwrite);
	}
	print_buffer(buffer);
	for (int index = 1; index < 50; index++) {

		char buf[15];
		buf[14] = '\0';
		int nread = read_from_buffer(buffer, buf, 14);
		printf("index:%d read_from_buffer size :%d \n", index,nread);
		printf("buffer is :%s \n", buf);
	}
	print_buffer(buffer);

    return 0;
}