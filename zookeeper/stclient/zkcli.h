/////////////////////单线程客户端///////////////////////////////////
#ifndef __ZKCLIENT_H__
#define __ZKCLIENT_H__

#include <zookeeper.h>
#include <zookeeper_log.h>

typedef struct zkclient zkclient;

//异步回调错误码
#define ZKRT_ERROR      -1
#define ZKRT_SUCCESS    0
#define ZKRT_NONODE     1    //节点/父节点不存在
#define ZKRT_NODEEXIST  2    //节点存在


//节点事件
#define EventNodeCreated             0
#define EventNodeDeleted             1
#define EventNodeDataChanged         2
#define EventNodeChildrenChanged     3
#define EventNodeSubFail             4 //注册事件失败
#define EventNodeFail                5 //出错  会话断开/监视点失效

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



//事件回调
typedef void (*nodeEventHandler)(zkclient* cli,int eventType,const char* path,void* context);



typedef void (*sessionEventHandler)(zkclient* cli,void* context);

//异步操作完成回调参数
typedef struct RtContext{
    zkclient* cli;
    void* context;
    char* path[256];
    union {
        createNodeRTHandler createRTHandler;
        setNodeRTHandler setRTHandler;
        getNodeRTHandler getRTHandler;
        deleteNodeRTHandler deleteRTHandler;
        getChildrenNodeRTHandler getChildrenRTHandler;

        nodeEventHandler wacher;
    };
    void* extr;
}RtContext;



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

int zkclientSubscribeEvent(zkclient* cli,char* path,int eventType,nodeEventHandler wacher,void* context);

#endif