
#include <stdio.h>
#include "../anet/include/iomp.h"
#include "../anet/include/tcpserver.h"



tcpServer* server;

void  onMessage(connection* con,char* buf,int size){
	printf("onMessage %s %d\n",buf,size);
	connectionSend(con,buf,size);
}

void onClose(connection* con){
	printf("close:%d \n",con->fd);
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