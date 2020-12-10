#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zookeeper.h>
#include <zookeeper_log.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>
#include "zkcli.h"
#define PRINTF printf

static const int RECV_TIMEOUT = 10000;


int zkclient_select(zkclient* cli);

typedef struct zkclient{
    zhandle_t* zh;
    clientid_t myid;
    int connected;
    int stop;
}zkclient;
/////////////////////////////////////////////////////工具函数/////////////////////////////////////////////////////////////////////////

//会话状态
static const char* state2String(int state){
  if (state == 0)
    return "CLOSED_STATE";
  if (state == ZOO_CONNECTING_STATE)
    return "CONNECTING_STATE";
  if (state == ZOO_ASSOCIATING_STATE)
    return "ASSOCIATING_STATE";
  if (state == ZOO_CONNECTED_STATE)
    return "CONNECTED_STATE";
  if (state == ZOO_READONLY_STATE)
    return "READONLY_STATE";
  if (state == ZOO_EXPIRED_SESSION_STATE)
    return "EXPIRED_SESSION_STATE";
  if (state == ZOO_AUTH_FAILED_STATE)
    return "AUTH_FAILED_STATE";

  return "INVALID_STATE";
}

//事件类型 会话/节点
static const char* type2String(int state){
  if (state == ZOO_CREATED_EVENT)
    return "CREATED_EVENT";
  if (state == ZOO_DELETED_EVENT)
    return "DELETED_EVENT";
  if (state == ZOO_CHANGED_EVENT)
    return "CHANGED_EVENT";
  if (state == ZOO_CHILD_EVENT)
    return "CHILD_EVENT";
  if (state == ZOO_SESSION_EVENT)
    return "SESSION_EVENT";
  if (state == ZOO_NOTWATCHING_EVENT)
    return "NOTWATCHING_EVENT";

  return "UNKNOWN_EVENT_TYPE";
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////监控函数/////////////////////////////////////////////////////////////////////////
static void global_wacher_fn(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx){
    zkclient* cli = watcherCtx;
    PRINTF("trigger event type:%s state:%s ",type2String(type),state2String(state));
    if(path != 0 && strlen(path) > 0){
        PRINTF("path:%s",path);
    }
    if(type == ZOO_SESSION_EVENT){
        //收到prime回应  可能是第一次正常连接 也有可能是在会话超时时间内重新连接
        if(state == ZOO_CONNECTED_STATE){
            const clientid_t * id = zoo_client_id(zh);
            if(cli->myid.client_id == 0 ||  id->client_id != cli->myid.client_id){
                cli->myid = *id;
                cli->connected = 1;
                PRINTF("new session id:%llx\n",id->client_id);
            }
        }
        else if(state == ZOO_EXPIRED_SESSION_STATE){ // 会话过期
            cli->connected = 0;
            PRINTF("session expire.\n");
            //回调给上层 上层进行过期逻辑处理.上层可以直接杀掉进程(有点暴力)或者重新连接并注册监视点。
        }
        else if(ZOO_AUTH_FAILED_STATE){
            cli->connected = 0;
            PRINTF("auth failed.\n");
        }
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





zkclient* zkclient_create(){
    zkclient* cli =  malloc(sizeof(zkclient));
    assert(cli != 0);
    cli->zh = 0;
    cli->connected = 0;
    cli->stop = 0;
    return cli;
}

int zkclient_is_connected(zkclient* cli){
    assert(cli != 0);
    return cli->connected == 1;
}


void zkclient_free(zkclient* cli){
    assert(cli);
    if(cli->zh){
        zookeeper_close(cli->zh);
    }
    free(cli);
    cli = 0;
}

//建立会话
int zkclient_connect(zkclient* cli,const char* host)
{
    assert(cli);
    zkclient* zh = zookeeper_init(host, global_wacher_fn, RECV_TIMEOUT, &(cli->myid), cli, 0);
    if (!zh) {
        return errno;
    }
    cli->zh = zh;
    return 0;
}

int zkclient_run(zkclient* cli){

    zkclient_select(cli);

}

int zkclient_stop(zkclient* cli){
    assert(cli);
    cli->stop = 1;
}


int zkclient_select(zkclient* cli){
    assert(cli);
    zhandle_t* zh = cli->zh;
    fd_set rfds, wfds, efds;
    FD_ZERO(&rfds); FD_ZERO(&wfds);  FD_ZERO(&efds);
    if(!cli->stop) {
        int fd;
        int interest;
        int events;
        struct timeval tv = {0,0};
        int rc;
        zookeeper_interest(zh, &fd, &interest, &tv);
        if (fd != -1) {
            if (interest&ZOOKEEPER_READ) {
                FD_SET(fd, &rfds);
            } else {
                FD_CLR(fd, &rfds);
            }
            if (interest&ZOOKEEPER_WRITE) {
                FD_SET(fd, &wfds);
            } else {
                FD_CLR(fd, &wfds);
            }
        } else {
            fd = 0;
        }
        FD_SET(0, &rfds);
        rc = select(fd+1, &rfds, &wfds, &efds, &tv);
        events = 0;
        if (rc > 0) {
            if (FD_ISSET(fd, &rfds)) {
                events |= ZOOKEEPER_READ;
            }
            if (FD_ISSET(fd, &wfds)) {
                events |= ZOOKEEPER_WRITE;
            }
        }
         
        zookeeper_process(zh, events);
    }

}
 