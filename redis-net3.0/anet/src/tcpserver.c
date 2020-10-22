
#include <stdlib.h>
#include "../include/tcpserver.h"
#include "../include/netapi.h"
#include "../include/iomp.h"


#define CHECK_SER_FD_ERR(server,fd)  \
if(fd < 0 || fd > server->cfg.maxfd )\
{ \
    PRINT_ERR("fd is over size")\
    return NET_RET_ERROR; \
} 

static int onTcpServerAcceptHandler(int lfd,void* udata);
static int onPassiveConnect(tcpServer* server,int fd);

static int onTcpServerAcceptHandler(int lfd,void* udata){
	tcpServer* server = udata;
    CHECK_PTR_ERR(server)

	char err[NET_ERR_LEN];
	int fd = netAccept(err,lfd);
	if (fd == NET_ERR)
	{
		PRINT_ERR(err);
		return NET_RET_ERROR;
	}
    
	return onPassiveConnect( server, fd);
}



tcpServer* tcpServerCreate(config* cfg){
    tcpServer* server = malloc(sizeof(tcpServer) );
    CHECK_PTR_RET_NULL(server)

	server->cfg.ip = cfg == 0 ? "127.0.0.1" : cfg->ip;
	server->cfg.port = cfg == 0 ? 8001 : cfg->port;
    server->cfg.maxfd = cfg == 0 ? 500 : cfg->maxfd;

    server->connectCallback = 0;
    server->disconnectCallback = 0;
    server->messageCallback = 0;

    server->conlist = malloc(sizeof(void*) * server->cfg.maxfd);
    CHECK_PTR_RET_NULL(server->conlist)

    for(int index = 0;index < server->cfg.maxfd;++index){
		server->conlist[index] = 0;
    }
    return server;
}



int  tcpServerStart(tcpServer* server) {
    CHECK_PTR_ERR(server)

	char err[NET_ERR_LEN];
	server->lfd = netTcpServer(err,server->cfg.ip,server->cfg.port);
    if(server->lfd ==  NET_RET_ERROR){
        PRINT_ERR(err);
        return NET_RET_ERROR;
    }
	if( NET_RET_ERROR == 
        evLoopRegister(server->lfd, EV_MASK_READ, onTcpServerAcceptHandler,server)){
        return NET_RET_ERROR;
    }
    return NET_RET_OK;
}



int  onPassiveConnect(tcpServer* server,int fd){
    CHECK_PTR_ERR(server)

    char err[NET_ERR_LEN];
	if( netSetNoblock(err,fd) == NET_ERR){
        PRINT_ERR(err)
        netClose(fd);
    }
    if( netTcpNoDelay(err,fd) == NET_ERR){
        PRINT_ERR(err)
        netClose(fd);
    }

    connection* con = connectionCreate(fd);
    CHECK_PTR_ERR(server)

    if( NET_RET_ERROR == 
     connectionInitCallBack( con,server->connectCallback, server->messageCallback, server->disconnectCallback)){
         return NET_RET_ERROR;
     }

    if( NET_RET_ERROR ==  connectionEstablished(con) ){
        return NET_RET_ERROR;
    }

    if( NET_RET_ERROR == tcpServerAddConnection(server,con) ){
        return NET_RET_ERROR;
    }
    return NET_RET_OK;
}


int tcpServerInitCallBack(tcpServer* server,ConnectCallback* connectCallback,
            MessageCallback* messageCallback,DisConnectCallback*  disconnectCallback){
    CHECK_PTR_ERR(server)

    server->connectCallback = connectCallback;
    server->disconnectCallback = messageCallback;
    server->messageCallback = disconnectCallback;
    return NET_RET_OK;
}


int tcpServerAddConnection(tcpServer* server, connection* con){
    CHECK_PTR_ERR(server)
    CHECK_PTR_ERR(con)
    CHECK_SER_FD_ERR(server,con->fd)


    server->conlist[con->fd] = con;
    return NET_RET_OK;
}

int tcpServerDelConnection(tcpServer* server, connection* con){
    CHECK_PTR_ERR(server)
    CHECK_PTR_ERR(con)
    CHECK_SER_FD_ERR(server,con->fd)

    server->conlist[con->fd] = 0;
    return NET_RET_OK;
}





