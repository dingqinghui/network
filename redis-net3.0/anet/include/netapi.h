#ifndef _NET_H__
#define _NET_H__

#include "../include/error.h"

#define NET_SK_BLOCK		1 
#define NET_SK_NOBLOCK		2

#define IP_MAX_LEN        32

#define NET_ERR             -1
#define NET_OK              0



int netClose(char* err,int fd);
int netTcpNoDelay( char* err,int fd);
int netSetNoblock(char* err,int fd);
int netSetTcpReuse(char* err,int fd);
int netTcpServer(char* err,char* ip, int port);
int netTcpCommonConnect(char* err,char* ip, int port, int flags);
int netTcpBlockConnect(char* err,char* ip, int port);
int netTcpNoBlockConnect(char* err,char* ip, int port);
int netAccept(char* err,int lfd);

int netSetSendBuf(char* err,int fd,int size);
int netSetRecvBuf(char* err,int fd,int size);
int netAcceptError(char* err);
int netIsSelfConnect(int sockfd);
#endif
