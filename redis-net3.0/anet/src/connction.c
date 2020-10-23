#include<string.h>
#include "../include/connction.h"
#include "../include/netapi.h"
#include  "../include/iomp.h"

char g_readBuf[65535];
char g_sendBuf[65535];


static int onReadHandler(int fd,void* udata);
static int onWriteHandler(int fd,void* udata);
static int onErrorHandler(int fd,void* udata);

static int onForceClose(connection* con);
static int onSetState(connection* con,int state);
static int isClosing(connection* con);

/*
    关闭：对于所有被动关闭，我们都默认对方使用了close而不是shutdown触发，所以本端对等执行close即可。
    主动关闭：将应用层发送缓冲区中的内容发送完毕即可，close后一定不可读了。
*/

static int onErrorHandler(int fd,void* udata){
    connection* con = udata;
	CHECK_PTR_ERR(con)
    PRINT_ERR(ERR_STR)
    onForceClose(con);
}



static int onReadHandler(int fd,void* udata){
    connection* con = udata;
    CHECK_PTR_ERR(con)

    int n = netRead(con->fd,g_readBuf,65535);
	if(n < 0){
		char err[NET_ERR_LEN];
		if( netIoError(err,con->fd) == NET_RET_ERROR ){
            PRINT_ERR(err)
			onForceClose(con);
            return NET_RET_ERROR;
		}
	}
    else if(n == 0){
        //对端shutdown(SHUT_WR) 或 close,无法确定是那种情况。不考虑shutdown(SHUT_WR) 半连接。 所以直接关闭，简化流程
       onForceClose(con);
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
                //write err
                PRINT_ERR(err)
				onForceClose(con);
                return NET_RET_ERROR;
			}
		}
		else{
			connectionSetSendBufSize(con,size - n);
            if(size <= n){
                //Send all the data in the half connect state
                if(isClosing(con) && onOutBufferIsEmpty(con) ){
                    onForceClose(con);
                }
                else{
                    evLoopUnregister(con->fd,EV_MASK_WRITE);
                }
            }
		}
	}

	return NET_RET_OK;
}

static int isClosing(connection* con){
     CHECK_PTR_ERR(con)
     return  con->state  == CON_STATE_CLOSING;
}

static int onSetState(connection* con,int state){
    CHECK_PTR_ERR(con)
    con->state = state;
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
    con->state == CON_STATE_CONNECTED;

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
    con = 0;
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

int onOutBufferIsEmpty(connection* con){
    return con->css == 0;
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

int connectionShutdown(connection* con ){
    CHECK_PTR_ERR(con)
    if(isClosing(con)){
        return NET_RET_OK;
    }
    //检测是否有要发送的数据
    if(onOutBufferIsEmpty(con)){
        return onForceClose(con);
    }
    onSetState(con,CON_STATE_CLOSING);
    return NET_RET_OK;
}



static int onForceClose(connection* con){
    CHECK_PTR_ERR(con)
    CHECK_PTR_ERR(con->disconnectCallback);
    //callback user logic
    con->disconnectCallback(con);
    //del event
    evLoopRemove(con->fd);
    //close fd
    char err[NET_ERR_LEN];
    if( netClose(err,con->fd) == NET_RET_ERROR){
        PRINT_ERR(err);
    }

    onSetState(con,CON_STATE_CLOSED);
    connectionFree(con);
}

