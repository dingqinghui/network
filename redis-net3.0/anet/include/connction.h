#ifndef __CONNECTION_H__
#define __CONNECTION_H__



#define CON_INIT 1
#define CON_CONNECTING 2 
#define CON_CONNECTED 3

#define CON_BUF_MAX_LEN  65535


struct connection;

typedef void  MessageCallback(struct connection*,char* buf,int size);
typedef void  DisConnectCallback(struct connection*);
typedef void  ConnectCallback(struct connection*);


typedef struct connection
{
	int fd;
	char* sbuffer;     		  //send cache
	char* rbuffer;     		  //read cache
	int css;                  //cur send size
	int crs;                  //cur read size

	ConnectCallback*    connectCallback;
	MessageCallback*    messageCallback;
	DisConnectCallback* disconnectCallback;
}connection;


connection* connectionCreate(int fd);
int  connectionFree(connection* con);

int connectionInitCallBack(connection* con, ConnectCallback* connectCallback,
		MessageCallback* messageCallback,DisConnectCallback*  disconnectCallback);
int connectionConnect(connection* con, char* ip,int port,int block);

int connectionSend(connection* con,char* buf,int size);
int connectionEstablished(connection* con );
int connectionClose(connection* con );


int getReadBuf(connection* con,char* buf);
int setReadBuf(connection* con,char* buf,int size);
int connectionGetSendBuf(connection* con,char* buf);
int connectionWriteSendBuf(connection* con,char* buf,int size);
void connectionSetSendBufSize(connection* con,int size);



#endif