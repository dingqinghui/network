
#include <stdio.h>
#include "../anet/include/tcpserver.h"
#include "../anet/include/tcpclient.h"

void  onMessage(tcpClient* cli,connection* con,char* buf,int size){
	tcpClientSend(cli,buf,size);
	printf("%s \n",buf);
}


int main()
{
	evLoopCraete(1000);

	tcpClient* cli = tcpClientCreate("127.0.0.1",8001 , 1);
	tcpClientCallback callback;
	callback.msgcb = onMessage;
	callback.connectcb = 0;
	callback.closecb = 0;
	tcpClientInitCallBack(cli,callback);
	tcpClientStart( cli );


	evLoopRun( EV_WAIT_FPS );
    return 0;
}