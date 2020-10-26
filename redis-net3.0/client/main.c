
#include <stdio.h>
#include "../anet/include/iomp.h"
#include "../anet/include/tcpclient.h"
#include "../anet/include/connction.h"
#include "../anet/include/tcpserver.h"
#include "../anet/include/buffer.h"




//tcpClient* cli = 0;
void  onMessage(connection* con,buffer* input){
	char buf[65535];
	int size = bufferRead(input, buf,65535);
	printf("onMessage:buf:%s size:%d\n",buf,size);
	connectionSend(con,buf,size);
}

void onClose(connection* con){
	printf("onClose:%d \n",con->fd);
}

#include <sys/socket.h>
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


//#include "../anet/include/buffer.h"
//int main()
//{
//	buffer* buff = bufferCreate(10);
//	for (int index = 0;index < 5;++index)
//	{
//		bufferWrite(buff, "1234,", 5);
//		bufferPrint(buff);
//	}
//	printf("========================================================\n");
//	int n = 0;
//	do 
//	{
//		char b[2];
//		n = bufferRead(buff, b, 2);
//		printf("%c%c\n", b[0], b[1]);
//		bufferPrint(buff);
//	} while (n > 0);
//
//	printf("========================================================\n");
//	for (int index = 0; index < 5; ++index)
//	{
//		bufferWrite(buff, "1234,", 5);
//		bufferPrint(buff);
//	}
//
//	printf("========================================================\n");
//	do
//	{
//		char b[2];
//		n = bufferRead(buff, b, 2);
//		printf("%c%c\n", b[0],b[1]);
//		bufferPrint(buff);
//	} while (n > 0);
//
//	printf("%c%c\n", buff->data[39], buff->data[40]);
//	return 0;
//}