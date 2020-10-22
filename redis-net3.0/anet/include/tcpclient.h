#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__


#include "../include/tcpcallback.h"
#include "../include/connction.h"
#include "../include/error.h"

struct connection;
typedef struct tcpClient{
    char* ip;
    int port;
    int block;
	struct connection* con;

    ConnectCallback*    connectCallback;
	MessageCallback*    messageCallback;
	DisConnectCallback* disconnectCallback;
}tcpClient;



tcpClient* tcpClientCreate(char* ip, int port, int block);
int tcpClientInitCallBack(tcpClient* cli, ConnectCallback* connectCallback,
            MessageCallback* messageCallback,DisConnectCallback*  disconnectCallback);
int tcpClientStart(tcpClient* );

#endif


