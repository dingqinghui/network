
#include "../include/tcpclient.h"
#include "../include/netapi.h"
#include "../include/connction.h"
#include "../include/iomp.h"

static int connectedFinish(tcpClient* cli,int fd);


static int onConnectHandler(int fd,void* udata){
    tcpClient* cli = udata;
    CHECK_PTR_ERR(cli)

    if( evLoopUnregister( fd, EV_MASK_WRITE)  == NET_RET_ERROR ){
        return NET_RET_ERROR;
    }

    if( connectedFinish( cli, fd) == NET_RET_ERROR){
        return NET_RET_ERROR;
    }
}



tcpClient* tcpClientCreate(char* ip,int port,int block)
{
    tcpClient* cli = malloc(sizeof(tcpClient));
    CHECK_PTR_RET_NULL(cli)

    cli->ip = ip;
    cli->port = port;
    cli->block =block;
    cli->con = 0;
    return cli;
}


int tcpClientStart(tcpClient* cli){
    CHECK_PTR_ERR(cli)

    char err[NET_ERR_LEN];
    int fd ;
    if(cli->block){
        fd = netTcpBlockConnect(err, cli->ip, cli->port);
    }
    else{
        fd = netTcpNoBlockConnect( err, cli->ip, cli->port);
    }

    if(fd ==  NET_RET_ERROR){
        PRINT_ERR(err)
        return NET_RET_ERROR;
    }

	if (netIsSelfConnect(fd)) {
		netClose(err,fd);
		return NET_RET_OK;
	}

    if(!cli->block){
        if( evLoopRegister( fd, EV_MASK_WRITE, onConnectHandler,cli) == NET_RET_ERROR ){
            return NET_RET_ERROR;
        }
    }
    else{
        if( connectedFinish(cli,fd) ==  NET_RET_ERROR){
            return NET_RET_ERROR;
        }
    }
    return NET_RET_OK;
}


static int connectedFinish(tcpClient* cli,int fd){
    connection* con = connectionCreate(fd);
    CHECK_PTR_ERR(con);

    if( connectionInitCallBack( con,cli->connectCallback, cli->messageCallback, cli->disconnectCallback) == NET_RET_ERROR ){
        return NET_RET_ERROR;
    }
    if( connectionEstablished(con) == NET_RET_ERROR ){
        return NET_RET_ERROR;
    }

    cli->con = con;
    return NET_RET_OK;
}


int tcpClientInitCallBack(tcpClient* cli, ConnectCallback* connectCallback,MessageCallback* messageCallback,DisConnectCallback*  disconnectCallback){
    CHECK_PTR_ERR(cli);

    cli->connectCallback = connectCallback;
    cli->disconnectCallback = messageCallback;
    cli->messageCallback = disconnectCallback;
    return NET_RET_OK;
}





