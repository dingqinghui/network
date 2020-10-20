
#include <stdio.h>
#include "../anet/include/tcpserver.h"
#include "../anet/include/tcpclient.h"


void  onMessage(tcpServer* server,connection* con,char* buf,int size){
	printf("onMessage %s %d\n",buf,size);
	tcpServerSend(server,con,buf,size);
}

void onClose(tcpServer* server,connection* con){
	printf("close:%d \n",con->fd);
}

void onConnect(tcpServer* server,connection* con){
	printf("onConnect:%d \n",con->fd);
}

int main()
{
	evLoopCraete(1000);

	tcpServer* server =  tcpServerCreate(0);

	tcpServerCallback callback;
	callback.connectcb = onConnect ;
	callback.closecb = onClose ;
	callback.msgcb = onMessage ;

	tcpServerInitCallBack(server,callback);
	tcpServerStart(server);


	evLoopRun( EV_WAIT_FPS );
    return 0;
}