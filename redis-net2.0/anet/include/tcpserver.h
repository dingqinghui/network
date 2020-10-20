#ifndef __NETSERVER_H__
#define __NETSERVER_H__


#include "../include/iomp.h"
#include "../include/connction.h"

struct tcpServer;

typedef void  MessageCallback(struct tcpServer* server,connection* con,char* buf,int size);
typedef void  CloseCallback(struct tcpServer* server,connection* con);
typedef void  ConnectCallback(struct tcpServer* server,connection* con);

typedef struct tcpServerCallback{
    ConnectCallback* connectcb;
	MessageCallback* msgcb;
	CloseCallback* closecb;
}tcpServerCallback;


typedef struct config
{
	char* ip;
	char* port;
	int   maxfd;      
}config; 



typedef struct tcpServer {
	config cfg;
	int lfd;
	tcpServerCallback callback;
	connection** conlist;
}tcpServer;



tcpServer* tcpServerCreate(config* cfg);
void tcpServerInitCallBack(tcpServer* server,tcpServerCallback callback);
int  tcpServerStart(tcpServer* server);
int tcpServerSend(tcpServer* server,connection* con, char* buf,int size);



#endif
