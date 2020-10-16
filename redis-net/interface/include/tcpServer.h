
#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "../../anet/include/connmgr.h"

typedef struct config
{
	char* ip;
	char* port;
	int   maxclient;
}config; 



typedef struct tcpServer {
	config cfg;
	int lfd;
	//MessageCallback* msgcb;
}tcpServer;



#endif