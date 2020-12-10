/////////////////////单线程客户端///////////////////////////////////
#ifndef __ZKCLIENT_H__
#define __ZKCLIENT_H__

typedef struct zkclient zkclient;

//异步回调错误码
#define ZKRT_ERROR      -1
#define ZKRT_SUCCESS    0
#define ZKRT_NONODE     1    //节点/父节点不存在
#define ZKRT_NODEEXIST  2    //节点存在


// //会话事件
// #define SESSION_EVENT_NEW       0   //新会话建立成功
// #define SESSION_EVENT_RECONNECT 1   //会话重连成功
// #define SESSION_EVENT_EXPIRE    2   //会话过期
// #define SESSION_EVENT_DISCONNECT 3  //断开连接

typedef void (*createNodeRTHandler)(zkclient* cli,int errCode,const char* path,const char* value,void* context);
typedef void (*setNodeRTHandler)(zkclient* cli,int errCode,const char* path,const struct Stat *stat,void* context);
typedef void (*getNodeRTHandler)(zkclient* cli,int errCode,const char* path,const char* buff,int bufflen,const struct Stat *stat,void* context);
typedef void (*deleteNodeRTHandler)(zkclient* cli,int errCode,const char* path,void* context);
typedef void (*getChildrenNodeRTHandler)(zkclient* cli,int errCode,const char* path,const struct String_vector *strings,void* context);

//异步操作完成回调参数
typedef struct RtContext{
    zkclient* cli;
    void* context;
    const char* path;
    union {
        createNodeRTHandler createRTHandler;
        setNodeRTHandler setRTHandler;
        getNodeRTHandler getRTHandler;
        deleteNodeRTHandler deleteRTHandler;
        getChildrenNodeRTHandler getChildrenRTHandler;
    };
}RtContext;


typedef void (*sessionEventHandler)(zkclient* cli,void* context);


zkclient* zkclientCreate(sessionEventHandler sessionHandler,void* sessionEventCtx);
void zkclientFree(zkclient* cli);

int zkclientConnect(zkclient* cli,const char* host);
int zkclientRun(zkclient* cli);
int zkclientStop(zkclient* cli);

int zkclientIsConnected(zkclient* cli);
int zkclientIsExpired(zkclient* cli);

int zkclientCreateNode(zkclient* cli,const char* path,const char* data,int len,int isTmp,int isSeq,createNodeRTHandler watcher,void* context);
int zkclientSetNode(zkclient* cli,const char* path,const char* buff,int bufflen,setNodeRTHandler watcher,void* context);
int zkclientGetNode(zkclient* cli,const char* path,getNodeRTHandler watcher,void* context);
int zkclientDelNode(zkclient* cli,const char* path,deleteNodeRTHandler watcher,void* context);
int zkclientGetChildrens(zkclient* cli,const char* path,getChildrenNodeRTHandler watcher,void* context);

#endif