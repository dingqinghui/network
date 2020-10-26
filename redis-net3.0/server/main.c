
#include <stdio.h>
#include "../anet/include/iomp.h"
#include "../anet/include/tcpserver.h"
#include "../anet/include/buffer.h"


tcpServer* server;

void  onMessage(connection* con, buffer* input) {
	char buf[65535];
	int size = bufferRead(input, buf, 65535);
	printf("onMessage:buf:%s size:%d\n", buf, size);
	connectionSend(con, buf, size);
}

void onClose(connection* con){
	printf("onClose:%d \n",con->fd);
	tcpServerDelConnection(server,con);
}

void onConnect(connection* con){
	printf("onConnect:%d \n",con->fd);
}


int main()
{
	evLoopCraete(1000);

	server =  tcpServerCreate(0);

	tcpServerInitCallBack(server,onConnect,onMessage,onClose);


	tcpServerStart(server);


	evLoopRun( EV_WAIT_FPS );
    return 0;
}