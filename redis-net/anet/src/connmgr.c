

#include "../include/connmgr.h"

static conmgr* mgr = 0;

void createConMgr(int maxcon){
    mgr = malloc(sizeof(conmgr));
    if(!mgr){
        return -1;
    }
    mgr->conlist = malloc(sizeof(connection*)* maxcon);
    if(mgr->conlist){
        free(mgr);
        return -1;
    }
    mgr->maxcon = maxcon;
    return 0;
}

int  closeConnction(connection* con) {
    if(con->fd < 0 || con->fd > mgr->maxcon){
        return -1;
    }
    mgr->conlist[con->fd] = 0;
    return 0;
}


connection* getConnection(int fd){
    if(fd < 0 || fd > mgr->maxcon){
        return 0;
    }
    return mgr->conlist[fd];
}

void  addConnction(connection* con) {
    if(con->fd < 0 || con->fd > mgr->maxcon){
        return -1;
    }
	mgr->conlist[con->fd] = con;
    return ;
}