#include<string.h>
#include "../include/connction.h"
#include "../include/netapi.h"
#include  "../include/iomp.h"

char g_readBuf[65535];
char g_sendBuf[65535];


static int onReadHandler(int fd,void* udata);
static int onWriteHandler(int fd,void* udata);
static int onErrorHandler(int fd,void* udata);



static void onClose(connection* con);


static int onErrorHandler(int fd,void* udata){
    connection* con = udata;
	CHECK_PTR_ERR(con)

    onClose(con);
}



static int onReadHandler(int fd,void* udata){
    connection* con = udata;
    CHECK_PTR_ERR(con)

    int n = netRead(con->fd,g_readBuf,65535);
	if(n <= 0){
		char err[NET_ERR_LEN];
		if( netIoError(err,con->fd) == NET_RET_ERROR 
            || n == 0 ){
			onClose(con);
		}
	}
    else{
        CHECK_PTR_ERR(con->messageCallback);
        con->messageCallback(con,g_readBuf,n);
    }   

    return NET_RET_OK;
}

static int onWriteHandler(int fd,void* udata)
{
    connection* con = udata;
    CHECK_PTR_ERR(con)

	int size = connectionGetSendBuf(con,g_sendBuf);
	if(size > 0){
		int n = netWrite(con->fd,g_sendBuf,size);
		if(n <= 0){
			char err[NET_ERR_LEN];
			if( netIoError(err,con->fd) == NET_ERR 
                || n == 0 ){
				onClose(con);
			}
		}
		else{
			connectionSetSendBufSize(con,size - n);
            if(size <= n){
                evLoopUnregister(con->fd,EV_MASK_WRITE);
            }
		}
	}

	return NET_RET_OK;
}



connection* connectionCreate(int fd){
    connection* con = malloc(sizeof(connection));
    CHECK_PTR_RET_NULL(con)

    con->crs = 0;
    con->css = 0;
    con->fd = fd;
    con->rbuffer = malloc(sizeof(char)*CON_BUF_MAX_LEN);
    con->sbuffer = malloc(sizeof(char)*CON_BUF_MAX_LEN);

    con->connectCallback = 0;
    con->disconnectCallback = 0;
    con->messageCallback = 0;

    if( evLoopRegister( con->fd, EV_MASK_READ, onReadHandler,con) == NET_RET_ERROR){
        PRINT_ERR("");
        return NET_RET_NULL;
    }
    if( evLoopRegister( con->fd, EV_MASK_ERROR, onErrorHandler,con) ){
        PRINT_ERR("");
        return NET_RET_NULL;
    }
    return con;
}

int connectionFree(connection* con){
    CHECK_PTR_RET_NULL(con)
    if(con->rbuffer) free(con->rbuffer);
    if(con->sbuffer) free(con->sbuffer);
    free(con);
    return NET_RET_OK;
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

int connectionGetSendBuf(connection* con,char* buf){
    memcpy(buf,con->sbuffer,con->css);
    return con->css;
}

void connectionSetSendBufSize(connection* con,int size){
     con->css = size;
}

int connectionWriteSendBuf(connection* con,char* buf,int size){
    if(size < 0 || size + con->css > CON_BUF_MAX_LEN){
        return -1;
    }
    memcpy(con->sbuffer + con->css,buf,size);
    con->css  += size;
    return 0;
}


int connectionInitCallBack(connection* con,ConnectCallback* connectCallback,
                MessageCallback* messageCallback,DisConnectCallback*  disconnectCallback){
    CHECK_PTR_ERR(con)

    con->connectCallback = connectCallback;
    con->disconnectCallback = messageCallback;
    con->messageCallback = disconnectCallback;
    return NET_RET_OK;
}


int connectionEstablished(connection* con ){
    CHECK_PTR_ERR(con)
    CHECK_PTR_ERR(con->connectCallback);

    con->connectCallback(con);
    return NET_RET_OK;
}




int connectionSend(connection* con,char* buf,int size){
    CHECK_PTR_ERR(con)
    if( connectionWriteSendBuf(con,buf,size) == NET_RET_ERROR ){
        PRINT_ERR("con write buf fail.");
        return NET_RET_ERROR;
    }
    if(evLoopRegister(con->fd, EV_MASK_WRITE, onWriteHandler,con) == NET_RET_ERROR){
        PRINT_ERR("con register write event fail.");
        return NET_RET_ERROR;
    }
    return NET_RET_OK;
}

int connectionClose(connection* con ){
    CHECK_PTR_ERR(con)
    
    evLoopRemove(con->fd);
    netClose(con->fd);
    connectionFree(con);
    return NET_RET_OK;
}

//内部调用回调给用户层
static void onClose(connection* con){
    CHECK_PTR_ERR(con)
    CHECK_PTR_ERR(con->disconnectCallback);

    con->disconnectCallback(con);

    evLoopRemove(con->fd);
    netClose(con->fd);
}