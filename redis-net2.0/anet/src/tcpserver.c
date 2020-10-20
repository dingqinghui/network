
#include <stdlib.h>
#include "../include/tcpserver.h"
#include "../include/netapi.h"

static char g_readBuf[65535];
static char g_sendBuf[65535];

int onTcpServerErrorHandler(int fd, void* udata);
int onTcpServerAcceptHandler(int fd, void* udata);
int onTcpServerWriteHandler(int fd, void* udata);
int onTcpServerReadHandler(int fd, void* udata);

void onTcpServerCloseCon(tcpServer* server, int fd);
void onTcpServerMessage(tcpServer* server, connection* con, char* buf, int size);
void onTcpServerConnect(tcpServer* server,connection* con);

int  onTcpServerAddConnection(tcpServer* server, connection* con);
connection* onTcpServerGetConnection(tcpServer* server, int fd);
int onTcpServerDelConnection(tcpServer* server, int fd);

connection* onPassiveConnect(tcpServer* server, int fd);


int onTcpServerErrorHandler(int fd,void* udata){
    tcpServer* server = udata;
    onTcpServerCloseCon(server,fd);
}

int onTcpServerAcceptHandler(int lfd,void* udata){
	tcpServer* server = udata;
	char err[NET_ERR_LEN];
	int fd = netAccept(err,lfd);

	if (fd == NET_ERR)
	{
		printf("accpet client connect %s",err);
		return -1;
	}
	onPassiveConnect(server,fd);
	return 0;
}


int onTcpServerWriteHandler(int fd,void* udata)
{
    tcpServer* server = udata;
    connection* con =  onTcpServerGetConnection(server,fd);
    if(!con){
        return ;
    }
	int size = connectionGetSendBuf(con,g_sendBuf);
	if(size > 0){
		int n = netWrite(con->fd,g_sendBuf,size);
		if(n <= 0){
			char err[NET_ERR_LEN];
			if( netIoError(err,con->fd) == NET_ERR || n == 0 ){
				onTcpServerCloseCon(server,con);
			}
		}
		else{
			connectionSetSendBufSize(con,size - n);
            if(size <= n){
                evLoopUnregister(con->fd,EV_WRITE);
            }
		}
	}

	return 0;
}


int onTcpServerReadHandler(int fd,void* udata)
{
	tcpServer* server = udata;
    connection* con =  onTcpServerGetConnection(server,fd);
    if(!con){
        return ;
    }
	int n = netRead(con->fd,g_readBuf,65535);
	if(n <= 0){
		char err[NET_ERR_LEN];
		if( netIoError(err,con->fd) == NET_ERR || n == 0 ){
			onTcpServerCloseCon(server, fd);
			return -1;
		}
	}

	onTcpServerMessage(server,con, g_readBuf, n);
	return 0;
}



void onTcpServerCloseCon(tcpServer* server,int fd){
    connection* con = onTcpServerGetConnection(server,fd);
    if(!con){
        return ;
    }
    if( server->callback.closecb ){
        server->callback.closecb(server,con);
    }
    evLoopUnregister(con->fd,EV_READ|EV_WRITE|EV_ERROR);
    connectionFree(con);
    onTcpServerDelConnection(server,fd);
    close(fd);
}


void onTcpServerMessage(tcpServer* server, connection* con,char* buf,int size){
    if( server->callback.msgcb ){
        server->callback.msgcb(server,con,buf,size);
    }
}


void onTcpServerConnect(tcpServer* server,connection* con){
  
    if( server->callback.connectcb ){
        server->callback.connectcb(server,con);
    }
}



int onTcpServerAddConnection(tcpServer* server, connection* con){
    if( con->fd < 0 || con->fd > server->cfg.maxfd   )
    {
        return -1;
    }
    server->conlist[con->fd] = con;
    return 0;
}

int onTcpServerDelConnection(tcpServer* server, int fd){
    if( fd < 0 || fd > server->cfg.maxfd   )
    {
        return -1;
    }
    server->conlist[fd] = 0;
    return 0;
}

connection*  onTcpServerGetConnection(tcpServer* server,int fd){
    if( fd < 0 || fd > server->cfg.maxfd   )
    {
        return 0;
    }
    return server->conlist[fd];
}




tcpServer* tcpServerCreate(config* cfg){
    tcpServer* server = malloc(sizeof(tcpServer) );
    if(!server){
        return -1;
    }
	server->cfg.ip = cfg == 0 ? "127.0.0.1" : cfg->ip;
	server->cfg.port = cfg == 0 ? 8001 : cfg->port;
    server->cfg.maxfd = cfg == 0 ? 500 : cfg->maxfd;

    server->callback.msgcb = 0;
    server->callback.connectcb = 0;
    server->callback.closecb = 0;

    server->conlist = malloc(sizeof(void*) * server->cfg.maxfd);
    if(server->conlist == 0){
        return 0;
    }

    for(int index = 0;index < server->cfg.maxfd;++index){
		server->conlist[index] = 0;
    }

    return server;
}



int  tcpServerStart(tcpServer* server) {
	char err[NET_ERR_LEN];
	server->lfd = netTcpServer(err,server->cfg.ip,server->cfg.port);
    if(server->lfd < NET_ERR){
        printf("runServer: %s",err);
        return -1;
    }
	evLoopRegister(server->lfd, EV_READ, onTcpServerAcceptHandler,server);
    return 0;
}


connection*  onPassiveConnect(tcpServer* server,int fd){
    char err[NET_ERR_LEN];
    if( netSetNoblock(err,fd) == NET_ERR){
        netClose(fd);
        printf("passiveConnect: %s",err);
    }
    if( netTcpNoDelay(err,fd) == NET_ERR){
        netClose(fd);
        printf("passiveConnect: %s",err);
    }

    connection* con = connectionCreate(fd);
    if(!con){
        printf("driveConnect:con create fail \n");
        return 0;
    }

	evLoopRegister( con->fd, EV_READ, onTcpServerReadHandler,server) ;
    evLoopRegister( con->fd, EV_ERROR, onTcpServerErrorHandler,server) ;
    onTcpServerAddConnection(server,con);

    onTcpServerConnect(server,con);
    return con;
}


void tcpServerInitCallBack(tcpServer* server,tcpServerCallback callback){
    server->callback = callback;
}


int tcpServerSend(tcpServer* server,connection* con, char* buf,int size){
    if( connectionWriteSendBuf(con,buf,size) < 0 ){
        printf("tcpServerSend:write buf to conntion fail.\n");
        return -1;
    }
    return evLoopRegister(con->fd, EV_WRITE, onTcpServerWriteHandler,server);
}





