
#include <stdio.h>
#include "../anet/include/iomp.h"
#include "../anet/include/tcpclient.h"
#include "../anet/include/connction.h"
#include "../anet/include/tcpserver.h"
#include "../anet/include/buffer.h"
#include "../anet/include/timerheap.h"
#include "../anet/include/netapi.h"

static int clientCnt = 0;
static int packSize = 0;
static double sendTotal = 0;
static int beginTm = 0;
//tcpClient* cli = 0;
void  onMessage(connection* con,buffer* input){
	char buf[65535];
	int size = bufferRead(input, buf,65535);
	//printf("onMessage:buf:%s size:%d\n",buf,size);
	connectionSend(con,buf,size);
	sendTotal += size;
}

void onClose(connection* con){
	//printf("onClose:%d \n",con->fd);
}

#include <sys/socket.h>
void  OnConnect(connection* con){
	//printf("OnConnect:%d \n",con->fd);

	char sbuf[65535] = "122345679ss";
	connectionSend(con,sbuf,packSize);
}

static long long  getMillisecond(){
    long long  t = 0;
    struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (long long)tv.tv_sec * 1000;
	t += tv.tv_usec / 1000;
    return t;
}


void onTimer(void* udata){
	//long long interval = getMillisecond() - beginTm;

	printf("totalSend:%lluM totalRecv:%lluM\n",getWriteByte() / 1024 / 1024,getRecvByte() / 1024 / 1024);

	th_add_timer(1000,onTimer,0);
}
int main(int argc,char *argv[])
{
	if(argc >= 2){
		clientCnt =  atoi(argv[1]);
		packSize = atoi(argv[2]);
	}
	else{
		clientCnt = 1;
		packSize = 1024;
	}

	evLoopCraete(20000);
	th_init( 5 );


	for (size_t i = 0; i < clientCnt; i++)
	{
		tcpClient*cli = tcpClientCreate("127.0.0.1",8001 , 1);
	
		tcpClientInitCallBack(cli,OnConnect,onMessage,onClose);
		if( tcpClientStart(cli) == NET_RET_ERROR){
			printf("-----------------------------\n");
		}

	}

	th_add_timer(1000,onTimer,0);
	beginTm = getMillisecond();

	while(1){
		evLoopPoll();
		th_tick();
	}
	th_destroy();
	evLoopFree();
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



// int main()
// {

// 	th_timer_t*  timer5;
// 	th_timer_t*  timer8;

// 	th_init( 5 );
// 	for(int i = 11;i > 0;--i){
// 		printf("\n");
// 		printf("===========================================================\n");

// 		th_timer_t* timer = th_add_timer(i ,0,0);
// 		if(i == 5){
// 			timer5 = timer;
// 		}
// 		if(i == 8){
// 			timer8 = timer;
// 		}

		
// 		th_print();

// 		printf("===========================================================\n");
// 		printf("\n");
// 	}


// 	printf("\n");
// 	printf("===========================================================\n");
// 	th_del_timer(timer5);
// 	th_print();
// 	printf("===========================================================\n");
// 	printf("\n");


// 	printf("\n");
// 	printf("===========================================================\n");
// 	th_del_timer(timer8);
// 	th_print();
// 	printf("===========================================================\n");
// 	printf("\n");
// 	return 0;
// }