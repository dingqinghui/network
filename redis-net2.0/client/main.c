
#include <stdio.h>
#include "../anet/include/tcpserver.h"
#include "../anet/include/tcpclient.h"

void  onMessage(tcpClient* cli,connection* con,char* buf,int size){
	printf("buf:%s size:%d\n",buf,size);
	tcpClientSend(cli,buf,size);
}

void onClose(tcpClient* cli,connection* con){
	printf("close:%d \n",con->fd);
}

int main()
{
	evLoopCraete(1000);

	for (size_t i = 0; i < 100; i++)
	{
		tcpClient* cli = tcpClientCreate("127.0.0.1",8001 , 1);
		tcpClientCallback callback;
		callback.msgcb = onMessage;
		callback.connectcb = 0;
		callback.closecb = onClose;
		tcpClientInitCallBack(cli,callback);
		tcpClientStart( cli );
		char buf[] = "122345679ss";
		tcpClientSend(cli,buf,sizeof(buf));
	}
	
	evLoopRun( EV_WAIT_FPS );
    return 0;
}