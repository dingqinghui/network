
 #include <stdlib.h>
#include "../include/netserver.h"
#include "../include/handler.h"
#include "../include/netapi.h"




static netserver* server;


int createServer(config* cfg){
    server = malloc(sizeof(netserver) );
    if(!server){
        return -1;
    }

	if (cfg) {
		server->cfg.ip = cfg->ip;
		server->cfg.port = cfg->port;
        server->cfg.maxclient = cfg->maxclient;
        
	}
	else {
		server->cfg.ip = "127.0.0.1";
		server->cfg.port = 8001;
        server->cfg.maxclient = 1024;
	}

    createEventLoop(server->cfg.maxclient);
    createConMgr(server->cfg.maxclient);
    
    return 0;
}

void runLoop(int mod) {
    runEventLoop(mod);
}

void runServer(int mod) {
	char err[NET_ERR_LEN];
	server->lfd = netTcpServer(err,server->cfg.ip,server->cfg.port);
    if(server->lfd < NET_ERR){
        printf("runServer: %s",err);
        return 0;
    }
	registerEvent(server->lfd, EV_READ, acceptHandler);

	runLoop( mod);
}


connection*  passiveConnect(int fd){

    if(fd < 0 || fd > server->cfg.maxclient){
        netClose(fd);
        printf("passiveConnect:fd over size \n");
        return 0;
    }
    char err[NET_ERR_LEN];
    if( netSetNoblock(err,fd) == NET_ERR){
        netClose(fd);
        printf("passiveConnect: %s",err);
    }
    if( netTcpNoDelay(err,fd) == NET_ERR){
        netClose(fd);
        printf("passiveConnect: %s",err);
    }

    connection* con = createConnection(fd);
    if(!con){
        printf("driveConnect:con create fail \n");
        return 0;
    }

   
	registerEvent( con->fd, EV_READ, readHandler) ;


    setConnectState(con,CON_CONNECTED);

    addConnction(con);

    return con;
}

connection* driveConnect(char* ip,int port,int block)
{
    char err[NET_ERR_LEN];
    int fd ;
    if(block){
        fd = netTcpBlockConnect(err,ip, port);
    }
    else{
        fd = netTcpNoBlockConnect( err,ip, port);
    }
    if(fd < NET_ERR){
        printf("driveConnect %s",err);
        return 0;
    }

    if(fd > server->cfg.maxclient){
        netClose(fd);
        printf("driveConnect:fd over size \n");
        return 0;
    }

    connection* con = createConnection(fd);
    if(!con){
        printf("driveConnect:con create fail \n");
        return 0;
    }

    //addEvent(server->loop, con->fd, EV_WRITE, writeHandler);
	registerEvent( con->fd, EV_READ, readHandler) ;

    setConnectState( con,CON_CONNECTING);

    addConnction(con);
    return con;
}




void dispatchMessage(connection* con,char* buf,int size){
    if(server->msgcb){
        server->msgcb(con,buf,size);
    }
}


void initMessageCallback(MessageCallback* cb)
{
    server->msgcb = cb;
}

int tcpSend(connection* con,char* buf,int size){
    // 写入connection缓冲区 
    if( writeSendBuf(con,buf,size) < 0 ){
        printf("tcpSend:write buf to conntion fail.\n");
        return -1;
    }
    // 注册事件
    registerEvent(con->fd, EV_READ, readHandler);
    return 0;
}