

#include <sys/time.h>
#include <stdio.h>
#include "../include/netapi.h"
#include "../include/handler.h"
#include "../include/netserver.h"
#include "../include/connction.h"

static char g_readBuf[65535];
static char g_sendBuf[65535];

long bg = 0;


long getCurrentTime()  
{  
   struct timeval tv;  
   gettimeofday(&tv,NULL);  
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;  
} 

int acceptHandler(int lfd)
{

	char err[NET_ERR_LEN];
	int fd = netAccept(err,lfd);

	printf("acceptHandler fd:%d\n",fd);
	if (fd == NET_ERR)
	{
		printf("accpet client connect %s",err);
		return -1;
	}
	connection* con = passiveConnect(fd);

	return 0;
}


int writeHandler(int fd)
{
	printf("writeHandler fd:%d\n",fd);
	connection* con = getConnection(fd);
	if(!con){
		return -1;
	}
	if(isConnnecting(con)){
		char err[NET_ERR_LEN];
		if(netGetSocketErr(err,con->fd) == 0){
			setConnectState(con,CON_CONNECTED);
		}else{
			closeConnction(con);
		}
		return 0;
	}

	int size = getSendBuf(con,g_sendBuf);
	if(size > 0){
		int n = netWrite(con->fd,g_sendBuf,size);
		if(n <= 0){
			char err[NET_ERR_LEN];
			if( netIoError(err,con->fd) == NET_ERR || n == 0 ){
				closeConnction(con);
			}
		}
		else{
			setSendBufSize(con,size - n);
		}
	}

	return 0;
}


int readHandler(int fd)
{
	printf("readHandler fd:%d\n",fd);
	connection* con = getConnection(fd);
	if(!con){
		return ;
	}
	
	int n = netRead(con->fd,g_readBuf,65535);
	printf("readHandler size:%d\n",n);

	if(n <= 0){
		//fd error
		char err[NET_ERR_LEN];
		if( netIoError(err,con->fd) == NET_ERR || n == 0 ){
			closeConnction(con);
			
			return -1;
		}
	}

	dispatchMessage( con, g_readBuf,n);

	return 0;
}
