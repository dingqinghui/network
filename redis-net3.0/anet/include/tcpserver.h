#ifndef __NETSERVER_H__
#define __NETSERVER_H__



#include "../include/connction.h"


typedef struct config
{
	char* ip;
	int  port;
	int   maxfd;      
}config; 


typedef struct tcpServer {
	config cfg;
	int lfd;
	connection** conlist;

	ConnectCallback*    connectCallback;
	MessageCallback*    messageCallback;
	DisConnectCallback* disconnectCallback;
}tcpServer;



tcpServer* tcpServerCreate(config* cfg);
int tcpServerInitCallBack(tcpServer* server,ConnectCallback* connectCallback,
            MessageCallback* messageCallback,DisConnectCallback*  disconnectCallback);
int  tcpServerStart(tcpServer* server);

int tcpServerAddConnection(tcpServer* server, connection* con);
int tcpServerDelConnection(tcpServer* server, connection* con);

#endif
