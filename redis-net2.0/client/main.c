
#include <stdio.h>
#include "../anet/include/netserver.h"
#include "../anet/include/netapi.h"

void  onMessage(connection* con,char* buf,int size){
	tcpSend(con,buf,size);
	printf("%s \n",buf);
}


int main()
{

	createServer(0);
	initMessageCallback(onMessage);
	for (size_t i = 0; i < 1; i++)
	{

		connection* con =  driveConnect("127.0.0.1", 8001,1);
		if( con )
		{
			printf("send:%d\n",tcpSend(con,"123456789\n",10));
		}
		
	}
	
	runLoop(EV_WAIT_FPS) ;


    return 0;
}