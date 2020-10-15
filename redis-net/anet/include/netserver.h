#ifndef __NETSERVER_H__
#define __NETSERVER_H__


#include "../include/iomp.h"
#include "../include/connction.h"



typedef struct config
{
	char* ip;
	char* port;
	int   maxclient;
}config; 



typedef struct netserver {
	EventLoop* loop;
	connection** conslot;
	config cfg;
	int lfd;
}netserver;


int reverseSlotId();

int createServer(config* cfg);
void runServer(int mod);
void addConnction(connection* con);
int closeConnction(connection* con);
connection* getConnection(int fd);

connection*  passiveConnect(int fd);
connection* driveConnect(char* ip,int port,int block);

void runLoop(int mod);
#endif
