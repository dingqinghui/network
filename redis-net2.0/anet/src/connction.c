#include<string.h>
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
    con->rbuffer = malloc(sizeof(char)*CON_BUF_MAX_LEN);
    con->sbuffer = malloc(sizeof(char)*CON_BUF_MAX_LEN);

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


int getReadBuf(connection* con,char* buf){
    memcpy(buf,con->rbuffer,con->crs);
    return con->crs;
}

int setReadBuf(connection* con,char* buf,int size){
    if(size < 0 || size > CON_BUF_MAX_LEN){
        return -1;
    }
    memcpy(con->rbuffer,buf,size);
    con->crs  = size;
    return 0;
}

int getSendBuf(connection* con,char* buf){
    memcpy(buf,con->sbuffer,con->css);
    return con->css;
}

void setSendBufSize(connection* con,int size){
     con->css = size;
}

int writeSendBuf(connection* con,char* buf,int size){
    if(size < 0 || size + con->css > CON_BUF_MAX_LEN){
        return -1;
    }
    memcpy(con->sbuffer + con->css,buf,size);
    con->css  += size;
    return 0;
}
