
#include <stdio.h>
#include "../anet/include/iomp.h"
#include "../anet/include/tcpclient.h"
#include "../anet/include/connction.h"

//tcpClient* cli = 0;
void  onMessage(connection* con,char* buf,int size){
	printf("onMessage:buf:%s size:%d\n",buf,size);
	connectionSend(con,buf,size);
}

void onClose(connection* con){
	printf("onClose:%d \n",con->fd);
}

void  OnConnect(connection* con){
	printf("OnConnect:%d \n",con->fd);

	char sbuf[] = "122345679ss";
	//connectionSend(con,sbuf,sizeof(sbuf));
}


int main()
{
	evLoopCraete(1000);

	for (size_t i = 0; i < 1; i++)
	{
		tcpClient*cli = tcpClientCreate("127.0.0.1",8001 , 1);
	
		tcpClientInitCallBack(cli,OnConnect,onMessage,onClose);
		tcpClientStart(cli);
	}
	
	evLoopRun( EV_WAIT_FPS );
    return 0;
}