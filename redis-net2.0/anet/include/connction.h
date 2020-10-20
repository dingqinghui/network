#ifndef __CONNECTION_H__
#define __CONNECTION_H__



#define CON_INIT 1
#define CON_CONNECTING 2 
#define CON_CONNECTED 3

#define CON_BUF_MAX_LEN  65535


typedef struct connection
{
	int fd;
	char* sbuffer;     		  //send cache
	char* rbuffer;     		  //read cache
	int css;                  //cur send size
	int crs;                  //cur read size
}connection;


connection* connectionCreate(int fd);
void connectionFree(connection* con);

int getReadBuf(connection* con,char* buf);
int setReadBuf(connection* con,char* buf,int size);
int connectionGetSendBuf(connection* con,char* buf);
int connectionWriteSendBuf(connection* con,char* buf,int size);
void connectionSetSendBufSize(connection* con,int size);

#endif