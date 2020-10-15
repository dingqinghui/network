#include "../include/connction.h"


connection* createConnection(int fd){
    connection* con = malloc(sizeof(connection));
    if(!con){
        return 0;
    }
    //init 
    con->crs = 0;
    con->css = 0;
    con->fd = fd;
    con->rbuffer = malloc(sizeof(char)*65535);
    con->sbuffer = malloc(sizeof(char)*65535);

    setConnectState(con,CON_INIT);

    return con;
}



void freeConnection(connection* con){
    if(!con){
         return ;
    }

    if(con->rbuffer){
        free(con->rbuffer);
    }
    if(con->sbuffer){
        free(con->sbuffer);
    }
    free(con);
}


int setConnectState(connection* con,int state){
    con->state = state;
}


int isConnnecting(connection* con){
    return con->state == CON_CONNECTING;
}

