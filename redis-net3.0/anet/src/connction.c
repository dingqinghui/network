#include<string.h>
#include "../include/connction.h"
#include "../include/netapi.h"
#include  "../include/iomp.h"
#include "../include/buffer.h"
#include "../include/zmemory.h"

static int  onReportInfo(connection* con, char* actStr);

char G_READBUF[65535];
char G_SENDBUF[65535];

static long long  G_SEND_BYTE = 0;
static long long  G_RECV_BYTE = 0;
long long getRecvByte(){
    long long old = G_RECV_BYTE;
    G_RECV_BYTE = 0;
    return old;
}
long long getWriteByte(){
    long long old = G_SEND_BYTE;
    G_SEND_BYTE = 0;
    return old;
}

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
    NET_LOG_ERROR(ERR_STR);
    onForceClose(con);
}

static int onReadHandler(int fd,void* udata){
    connection* con = udata;
    CHECK_PTR_ERR(con)

    int n = netRead(con->fd,G_READBUF,65535);
	if(n < 0){
		char err[NET_ERR_LEN];
		if( netIoError(err,con->fd) == NET_RET_ERROR ){
			onForceClose(con);
            return NET_RET_ERROR;
		}
	}
    else if(n == 0){
        //对端shutdown(SHUT_WR) 或 close,无法确定是那种情况。不考虑shutdown(SHUT_WR) 半连接。 所以直接关闭，简化流程
       onForceClose(con);
    }
    else{
        G_RECV_BYTE += n;
		bufferWrite(con->inputBuf, G_READBUF, n);
        if(con->messageCallback){
            con->messageCallback(con, con->inputBuf );
        }
    }   

    return NET_RET_OK;
}

static int onWriteHandler(int fd,void* udata)
{
    connection* con = udata;
    CHECK_PTR_ERR(con)

	int size = bufferRead(con->outputBuf,G_SENDBUF,65535);
	if(size > 0){
		int n = netWrite(con->fd, G_SENDBUF,size);
		if(n <= 0){
			char err[NET_ERR_LEN];
			if( netIoError(err,con->fd) == NET_ERR 
                || n == 0 ){
                //write err
                NET_LOG_ERROR(err);
				onForceClose(con);
                return NET_RET_ERROR;
			}
		}
		else{
            G_SEND_BYTE += n;
			bufferWrite(con->outputBuf, G_SENDBUF + n, size - n);
            if(size <= n){
                //Send all the data in the half connect state
                if(isClosing(con) && bufferIsEmpty(con->outputBuf)){
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
    connection* con = zmalloc(sizeof(connection));


    con->fd = fd;
	con->connectCallback = 0;
	con->disconnectCallback = 0;
	con->messageCallback = 0;
	con->state == CON_STATE_CONNECTED;

	con->outputBuf = bufferCreate(SEND_BUF_INIT_VAL);
	if (!con->outputBuf) {
		zfree(con);
		return NET_RET_NULL;
	}
	con->inputBuf = bufferCreate(RECV_BUF_INIT_VAL);
	if (!con->inputBuf) {
		zfree(con->outputBuf);
		zfree(con);
		return NET_RET_NULL;
	}

    if( evLoopRegister( con->fd, EV_MASK_READ, onReadHandler,con) == NET_RET_ERROR){
		zfree(con->inputBuf);
		zfree(con->outputBuf);
		zfree(con);
        return NET_RET_NULL;
    }
    if( evLoopRegister( con->fd, EV_MASK_ERROR, onErrorHandler,con) ){
		zfree(con->inputBuf);
		zfree(con->outputBuf);
		zfree(con);
        return NET_RET_NULL;
    }

	//onReportInfo( con,"create new connection");

    return con;
}

int connectionFree(connection* con){
    CHECK_PTR_RET_NULL(con)
    if(con->inputBuf) bufferFree(con->inputBuf);
    if(con->outputBuf) bufferFree(con->outputBuf);
    zfree(con);
    return NET_RET_OK;
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
    int nsend = 0;
    if( bufferIsEmpty(con->outputBuf)  ){
        nsend = netWrite(con->fd, buf,size);
        if(nsend <= 0){
            char err[NET_ERR_LEN];
            if( netIoError(err,con->fd) == NET_ERR 
                || nsend == 0 ){
                //write err
                NET_LOG_ERROR(err);
                onForceClose(con);
                return NET_RET_ERROR;
            }
        }
    }
    G_SEND_BYTE += nsend;
    if(size - nsend){
        if(bufferWrite(con->outputBuf, buf + nsend, size - nsend) == NET_RET_ERROR ){
            NET_LOG_ERROR("con write buf fail.");
            return NET_RET_ERROR;
        }
        if(evLoopRegister(con->fd, EV_MASK_WRITE, onWriteHandler,con) == NET_RET_ERROR){
            NET_LOG_ERROR("con register write event fail.");
            return NET_RET_ERROR;
        }
    }
  
    return NET_RET_OK;
}

int connectionShutdown(connection* con ){
    CHECK_PTR_ERR(con)
    if(isClosing(con)){
        return NET_RET_OK;
    }
    //检测是否有要发送的数据
    if(bufferIsEmpty(con->outputBuf)){
        return onForceClose(con);
    }
    onSetState(con,CON_STATE_CLOSING);

	onReportInfo(con, "shutdown connection");
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
        NET_LOG_ERROR(err);
    }

    onSetState(con,CON_STATE_CLOSED);
    connectionFree(con);

	return NET_RET_OK;
}

static int  onReportInfo(connection* con,char* actStr) {
	CHECK_PTR_ERR(con)

    char err[NET_ERR_LEN];

	char localIp[IP_MAX_LEN];
	int localPort;
	netGetLocalInfo(con->fd, localIp, &localPort);


	char peerIp[IP_MAX_LEN];
	int peerPort;
	netGetPeerInfo(con->fd, peerIp, &peerPort);

	NET_LOG_DEBUG("%s.<fd:%d localAddr-%s:%d peerAddr-%s:%d>\n",
		actStr,con->fd, localIp, localPort, peerIp, peerPort);

	return NET_RET_OK;
}