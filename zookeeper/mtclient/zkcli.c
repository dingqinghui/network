#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zookeeper_log.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>

#include "zkcli.h"




int report_err(const char *fmt,...){
    char* buf[256];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, 256, fmt, ap);
    va_end(ap);

    printf(buf);
}



zkclient* zkclient_create(){
    zkclient* cli =  malloc(sizeof(zkclient));
    cli->handle = 0;
    cli->connected = 0;
    cli->expired = 0;
    cli->session_expire_timeout = 0;
    cli->session_cb = 0;
    return cli;
}


void zkclient_free(zkclient* cli){
    assert(cli);
    if(cli->handle){
        zookeeper_close(cli->handle);
    }
    free(cli);
    cli = 0;
}


void main_watcher (zhandle_t *zkh,int type,int state,const char *path,void* context)
{
    zkclient* cli = context;
    //会话事件
    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) { //连接完成
            cli->connected = 1;
            cli->session_expire_timeout = zoo_recv_timeout(zkh);
        } else if (state == ZOO_NOTCONNECTED_STATE ) {
            cli->connected = 0;
        } else if (state == ZOO_EXPIRED_SESSION_STATE) {
            cli->expired = 1;
            cli->connected = 0;
            //zookeeper_close(zkh);
        }

        ((sessionCallback)cli->session_cb)(cli);
    }
}


int zkclient_init(zkclient* cli ,char* host,int timeout,sessionCallback scb){
    assert(cli);

    cli->session_cb = scb;

    zoo_set_debug_level(ZOO_LOG_LEVEL_INFO);

    //这个时间不是真正的超时时间  服务器收到客户端发送的时间会调整会话超时时间
    zhandle_t* zh = zookeeper_init(host,main_watcher,timeout,  0,cli,0);
    if(!zh){
        report_err("init zookeeper fail.%s",strerror(errno));
        return -1;
    }

    cli->handle = zh;

    return 0;
}



int zkclient_is_expire(zkclient* cli){
    assert(cli);
    return cli->expired == 1;
}
int zkclient_is_connected(zkclient* cli){
    assert(cli);
    return cli->connected == 1;
}



static void create_node_completion(int rc, const char *rvalue, const void *data) {
    wacher_context* wc = data;
    wc->rc = rc;
    wc->rvalue = rvalue;
    wc->wcb(wc);
    free(wc);
}


int zkclient_create_node(zkclient* cli,char* path,void* value,int valuelen,wacherCallback wcb,void* context){
    assert(cli);
    wacher_context* wc = malloc(sizeof(wacher_context));
    wc->path = path;
    wc->context = context;
    wc->value = value;
    wc->cli = cli;
    wc->wcb = wcb;

    int rc = zoo_acreate(cli->handle,path,value,valuelen,&ZOO_OPEN_ACL_UNSAFE,ZOO_EPHEMERAL,create_node_completion,wc);
    if(rc != ZOK){
        report_err("create node fail.%s\n",zerror(rc));
        return -1;
    }
    return 0;
}


 static void set_data_completion(int rc, const struct Stat *stat,const void *data){
    wacher_context* wc = data;
    wc->rc = rc;
    wc->rvalue = 0;
    wc->wcb(wc);
    free(wc);
 }



int zkclient_set_data(zkclient* cli,char* path,void* value,int valuelen,wacherCallback wcb,void* context){
    assert(cli);
    wacher_context* wc = malloc(sizeof(wacher_context));
    wc->path = path;
    wc->context = context;
    wc->value = value;
    wc->cli = cli;
    wc->wcb = wcb;

    int rc = zoo_aset(cli->handle,path,value,valuelen,-1,create_node_completion,wc);
    if(rc != ZOK){
        report_err("create node fail.%s\n",zerror(rc));
        return -1;
    }
    return 0;
}

int zkclientSynExistNode(zkclient* cli,const char* path){
     char retPath[250];
     struct Stat stat;
     zoo_exists(cli->handle, path, 0, stat);
}