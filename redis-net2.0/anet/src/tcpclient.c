
#include "../include/tcpclient.h"
#include "../include/netapi.h"
#include "../include/connction.h"
#include "../include/iomp.h"

char g_readBuf[65535];
char g_sendBuf[65535];

int onTcpClienErrorHandler(int fd, void* udata);
int onTcpClientConnectHandler(int fd, void* udata);
int onTcpClientWriteHandler(int fd, void* udata);
int onTcpClientReadHandler(int fd, void* udata);

void onTcpClientClose(tcpClient* cli);
void onTcpClientConnect(tcpClient* cli);
void onTcpClientMessage(tcpClient* cli, char* buf, int size);


int onTcpClienErrorHandler(int fd,void* udata){
    tcpClient* cli = udata;
    onTcpClientClose(cli);
}

int onTcpClientConnectHandler(int fd,void* udata){
	tcpClient* cli = udata;
	if(!cli){
		return -1;
	}

    char err[NET_ERR_LEN];
    if(netGetSocketErr(err, cli->con->fd) == 0){
        onTcpClientConnect(cli);
    }else{
		onTcpClientClose(cli);
    }
    return 0;
}

int onTcpClientWriteHandler(int fd,void* udata)
{
    tcpClient* cli = udata;
    connection* con = cli->con;
	int size = connectionGetSendBuf(cli->con,g_sendBuf);
	if(size > 0){
		int n = netWrite(con->fd,g_sendBuf,size);
		if(n <= 0){
			char err[NET_ERR_LEN];
			if( netIoError(err,con->fd) == NET_ERR || n == 0 ){
				onTcpClientClose(cli);
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


int onTcpClientReadHandler(int fd,void* udata)
{
	tcpClient* cli = udata;
	if(!cli){
		return ;
	}
	connection* con = cli->con;
	int n = netRead(con->fd,g_readBuf,65535);
	if(n <= 0){
		char err[NET_ERR_LEN];
		if( netIoError(err,con->fd) == NET_ERR || n == 0 ){
			onTcpClientClose(cli);
			return -1;
		}
	}

	onTcpClientMessage(cli, g_readBuf, n);
	return 0;
}



void onTcpClientClose(tcpClient* cli){
    connection* con = cli->con;
    if(!con){
        return ;
    }
    if( cli->callback.closecb ){
        cli->callback.closecb(cli,con);
    }
    netClose(con->fd);
    evLoopUnregister(con->fd,EV_READ|EV_WRITE|EV_ERROR);
    connectionFree(con);
}

void onTcpClientConnect(tcpClient* cli){
   connection* con = cli->con;
    if(!con){
        return ;
    }
    if( cli->callback.connectcb ){
        cli->callback.connectcb(cli,con);
    }
}

void onTcpClientMessage(tcpClient* cli,char* buf,int size){
    connection* con = cli->con;
    if(!con){
        return ;
    }
    if( cli->callback.msgcb ){
        cli->callback.msgcb(cli,con,buf,size);
    }
}






tcpClient* tcpClientCreate(char* ip,int port,int block)
{
    tcpClient* cli = malloc(sizeof(tcpClient));
    if(!cli){
        return 0;
    }
    cli->ip = ip;
    cli->port = port;
    cli->block =block;
	cli->con = 0;
	
    cli->callback.msgcb = 0;
    cli->callback.connectcb = 0;
    cli->callback.closecb = 0;
    return cli;
}


int tcpClientStart(tcpClient* cli){
    char err[NET_ERR_LEN];
    int fd ;
    if(cli->block){
        fd = netTcpBlockConnect(err, cli->ip, cli->port);
    }
    else{
        fd = netTcpNoBlockConnect( err, cli->ip, cli->port);
    }
    if(fd < NET_ERR){
        printf("driveConnect %s",err);
        return -1;
    }

    cli->con = connectionCreate(fd);
    if(!cli->con){
        printf("driveConnect:con create fail \n");
        return -1;
    }
    if(!cli->block){
        evLoopRegister( cli->con->fd, EV_WRITE, onTcpClientConnectHandler,cli) ;
    }
    else{
        evLoopRegister( cli->con->fd, EV_READ, onTcpClientReadHandler,cli) ;
    }

    evLoopRegister(cli->con->fd, EV_ERROR, onTcpClienErrorHandler,cli) ;
	
    return 0;
}



int tcpClientSend(tcpClient* cli,char* buf,int size){
    if( connectionWriteSendBuf(cli->con,buf,size) < 0 ){
        printf("tcpClientSend:write buf to conntion fail.\n");
        return -1;
    }
    return evLoopRegister(cli->con->fd, EV_WRITE, onTcpClientWriteHandler,cli);
}



void tcpClientInitCallBack(tcpClient* cli,tcpClientCallback callback){
    cli->callback = callback;
}






