#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__


#include "../include/tcpcallback.h"
#include "../include/connction.h"

struct tcpClient;

typedef void  cliMessageCallback(struct tcpClient* cli,connection* con,char* buf,int size);
typedef void  cliCloseCallback(struct tcpClient* cli,connection* con);
typedef void  cliConnectCallback(struct tcpClient* cli,connection* con);


typedef struct tcpClientCallback{
    cliConnectCallback* connectcb;
	cliMessageCallback* msgcb;
	cliCloseCallback* closecb;
}tcpClientCallback;



struct connection;
typedef struct tcpClient{
    char* ip;
    int port;
    int block;
	struct connection* con;
    tcpClientCallback callback;
}tcpClient;



tcpClient* tcpClientCreate(char* ip, int port, int block);
void tcpClientInitCallBack(tcpClient* cli,tcpClientCallback callback);
int tcpClientStart(tcpClient* );

int tcpClientSend(tcpClient* cli,char* buf,int size);



#endif


