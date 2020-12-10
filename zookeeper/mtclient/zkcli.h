#ifndef __ZKCLI__H__
#define __ZKCLI__H__

#include <zookeeper.h>

typedef struct zkclient{
    zhandle_t* handle;
    int connected ;
    int expired ;
    int session_expire_timeout;   //连接状态下才能获取 会话过期时间 每次重连这个值都可能会变化
    void* session_cb;             //会话状态改变回调
}zkclient;


struct wacher_context;
typedef void (*wacherCallback)(struct wacher_context*);
 
typedef struct wacher_context{
    zkclient* cli;
    const char* path;
    const char* rvalue;
    void* context;
    void* value;
    int rc;
    wacherCallback wcb;
}wacher_context;


typedef void (*sessionCallback)(zkclient*);
    




zkclient* zkclient_create();
void zkclient_free(zkclient* cli);

int zkclient_init(zkclient* cli ,char* host,int timeout,sessionCallback scb);
//创建节点
int zkclient_create_node(zkclient* cli,char* path,void* value,int valuelen,wacherCallback wcb,void* context);
//改变节点数据
//删除节点


//监控节点



int zkclient_is_expire();
int zkclient_is_connected();
#endif