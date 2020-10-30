
#include <stdio.h>
#include "servermanager.h"
#include "connection.h"
#include "listener.h"
#include "ring_buffer.h"
#include "config.h"
#include "tcpclient.h"

static int clientCnt = 0;
static int packSize = 0;

void onMessage(connection *conn)
{
    int size = 0;
    char* msg = ring_buffer_get_msg(conn->ring_buffer_read, &size);
    printf("read all : %s, %d\n", msg, size);

    char buf[] = "abcd";
    connection_send(conn, buf, sizeof(buf) - 1);

    connection_send(conn, msg, size);
}

void onConnection(connection* conn)
{
    printf("connected!!! ip:port %s:%d\n", conn->client_ip, conn->client_port);
}

int main(int argc, char* argv[])
{
    if(argc >= 2){
		clientCnt =  atoi(argv[1]);
		packSize = atoi(argv[2]);
	}
	else{
		clientCnt = 1;
		packSize = 1024;
	}
  
   event_loop* loop = event_loop_create();
   
   tcpclient* cli = tcpclient_create("127.0.0.1",8001);

   event_loop_run(loop);

	return 0;
}


