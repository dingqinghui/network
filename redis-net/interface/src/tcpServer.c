

#include "../../kernel/include/netapi.h"
#include "../include/tcpServer.h"
#include "../../reactor/include/iomp.h"
#include "../../anet/include/handler.h"

tcpServer* createTcpServer(config* cfg){
    tcpServer*  server = malloc(sizeof(tcpServer) );
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

    createConMgr(server->cfg.maxclient);
    //server->msgcb = 0;
    return 0;
}



void startTcpServer(tcpServer* server) {
	char err[NET_ERR_LEN];
	server->lfd = netTcpServer(err,server->cfg.ip,server->cfg.port);
    if(server->lfd < NET_ERR){
        printf("runServer: %s",err);
        return 0;
    }

	registerEvent(server->lfd, EV_READ, acceptHandler);
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

    //addEvent(server->loop, con->fd, EV_WRITE, writeHandler);
	addEvent(server->loop, con->fd, EV_READ, readHandler) ;


    setConnectState(con,CON_CONNECTED);

    addConnction(con);

    return con;
}