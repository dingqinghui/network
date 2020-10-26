#ifndef __CONNECTION_H__
#define __CONNECTION_H__


#define SEND_BUF_INIT_VAL 4096
#define RECV_BUF_INIT_VAL 4096

#define CON_STATE_CONNECTED 	1
#define CON_STATE_CLOSING   	2
#define CON_STATE_CLOSED		3

struct buffer;
struct connection;

typedef void  MessageCallback(struct connection*, struct buffer* buf);
typedef void  DisConnectCallback(struct connection*);
typedef void  ConnectCallback(struct connection*);


typedef struct connection
{
	int fd;
	struct buffer* outputBuf;     		  
	struct buffer* inputBuf;
	int state;				

	ConnectCallback*    connectCallback;
	MessageCallback*    messageCallback;
	DisConnectCallback* disconnectCallback;
}connection;


connection* connectionCreate(int fd);
int  connectionFree(connection* con);

int connectionInitCallBack(connection* con, ConnectCallback* connectCallback,
		MessageCallback* messageCallback,DisConnectCallback*  disconnectCallback);


int connectionSend(connection* con,char* buf,int size);
int connectionEstablished(connection* con );
int connectionShutdown(connection* con );


int getReadBuf(connection* con,char* buf);
int setReadBuf(connection* con,char* buf,int size);
int connectionGetSendBuf(connection* con,char* buf);
int connectionWriteSendBuf(connection* con,char* buf,int size);
void connectionSetSendBufSize(connection* con,int size);



#endif