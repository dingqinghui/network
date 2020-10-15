

#include <sys/time.h>
#include <stdio.h>
#include "../include/netapi.h"
#include "../include/handler.h"
#include "../include/netserver.h"
#include "../include/connction.h"



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
	//printf("writeHandler fd:%d\n",fd);
	connection* con = getConnection(fd);
	if(!con){
		return ;
	}
	if(isConnnecting(con)){
		if(netGetSocketErr(con->fd) == 0){
			setConnectState(con,CON_CONNECTED);
		}else{
			//err
			closeConnction(con);
		}
	}

	// int n = netWrite(con->fd,con->css,65535);
	// if(n <= 0){
	// 	//fd error
	// 	if( netIoError(con->fd) == NET_ERR || n == 0 ){
	// 		closeConnction(con);
	// 	}
	// }

	return 0;
}


int readHandler(int fd)
{
	printf("readHandler fd:%d\n",fd);
	connection* con = getConnection(fd);
	if(!con){
		return ;
	}
	char buf[65535];
	int n = netRead(con->fd,buf,65535);
	if(n <= 0){
		//fd error
		char err[NET_ERR_LEN];
		if( netIoError(err,con->fd) == NET_ERR || n == 0 ){
			closeConnction(con);
		}
	}

	//printf("%s\n",buf);
	return 0;
}


