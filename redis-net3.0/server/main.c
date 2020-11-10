
#include <stdio.h>
#include "../anet/include/iomp.h"
#include "../anet/include/tcpserver.h"
#include "../anet/include/buffer.h"

 #include <sys/time.h>

static long long conbeginTM = 0;
static long long beginTM = 0;
static long long totalRecv = 0;
static long long totalSend = 0;
static long long connectCnt = 0;

static long long  getMillisecond(){
    long long  t = 0;
    struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (long long)tv.tv_sec * 1000;
	t += tv.tv_usec / 1000;
    return t;
}

tcpServer* server;

void  onMessage(connection* con, buffer* input) {
	char buf[65535];
	int size = bufferRead(input, buf, 65535);
	//printf("onMessage:buf:%s size:%d\n", buf, size);
	connectionSend(con, buf, size);
	totalRecv += size;
}

void onClose(connection* con){
	connectCnt--;
	//printf("onClose:%d \n",con->fd);
	tcpServerDelConnection(server,con);
}

void onConnect(connection* con){
	//printf("onConnect:%d \n",con->fd);
	 ++connectCnt;
	// if(connectCnt == 1){
	// 	conbeginTM = getMillisecond();
	// }
	// if(connectCnt >= 10000){
	// 	long long interval = getMillisecond() - conbeginTM;
	// 	int connectRate = connectCnt * 1000 / interval;
	// 	printf("onnectRate:%d/s alltm:%d/ms\n",connectRate,interval );
	// }
}



void onTimer(void* udata){
	//long long interval = getMillisecond() - beginTM;
	//double recvM = totalRecv * 1000 / interval /(1024*1024);
	
	printf("totalSend:%lluM totalRecv:%lluM  connectCnt:%d\n",getWriteByte() / 1024 / 1024,getRecvByte() / 1024 / 1024,connectCnt);

	th_add_timer(1000,onTimer,0);
}




int main()
{

	netloger_init();

	evLoopCraete(20000);
	th_init( 5 );
	th_add_timer(1000,onTimer,0);



	config cfg;
	cfg.ip = "127.0.0.1";
	cfg.port = 8001;
	cfg.maxfd = 20000;

	server =  tcpServerCreate(&cfg);

	tcpServerInitCallBack(server,onConnect,onMessage,onClose);


	tcpServerStart(server);

	beginTM = getMillisecond();

	while(1){
		th_tick();
		evLoopPoll();
	}
	
    return 0;
}