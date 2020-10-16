
#include <stdio.h>
#include "../anet/include/netserver.h"
#include "../anet/include/netapi.h"

void  onMessage(connection* con,char* buf,int size){
	printf("%s \n",buf);
	tcpSend(con,buf,size);
}




int main()
{
	createServer(0);
	initMessageCallback(onMessage);
	runServer(EV_WAIT_FPS);

    return 0;
}