#include<stdio.h>
#include <stdlib.h>
#include <string.h>


#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>
#include "zkcli.h"

#define PRINTF printf

#define CHECK_RC(rc,str)  \
if(rc != ZOK){ \
    reportErr("%s. rc:%s\n",str,zerror(rc));\
    return -1;\
}

#define CHECK_CONNECTED(cli) \
if(!zkclientIsConnected(cli)){ \
  PRINTF("error zkclient state is %d",zkclientGetState(cli));\
  return -1; \
}

static int zkclientSelect(zkclient* cli);
static int reportErr(const char *fmt,...);
static void globalWacherFn(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx);
static int getMode(int isTmp,int isSeq);
static int64_t OnGetCurrentMs() ;

static int zkclientWExist(zkclient* cli,RtContext* rtCx,char* path,watcher_fn fn ,stat_completion_t dc);
static int zkclientWGetChildrens(zkclient* cli,RtContext* rtCx,char* path,watcher_fn fn );
static int zkclientSetState(zkclient* cli,int state);
static void zkclientcConnected(zkclient* cli);
static void zkclientcConnecting(zkclient* cli);

static void getChildrenCompletion(int rc, const struct String_vector *strings,const void *data) ;
static void deleteNodeCompletion(int rc, const void *data);
static void getDataCompletion(int rc, const char *value, int value_len,const struct Stat *stat, const void *data);
static void setDataCompletion(int rc, const struct Stat *stat,const void *data);
static void createNodeCompletion(int rc, const char *value, const void *data) ;
static void existCompletion(int rc, const struct Stat *stat,const void *data);

static void eventWatcher(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx);
static int checkTriggerEvent(RtContext* rtCx,int type);
static int onSubscribeEvent(RtContext* rtCx);


typedef struct zkclient{
    zhandle_t* zh;
    clientid_t myid;
    int stop;
    sessionConnectedHandler connectHandle;
    sessionCloseHandler closeHandle;
    int sessionState;
    int sessionExpireTimeout;
    int connectingStartTime;
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

void dumpStat(const struct Stat *stat) {
    char tctimes[40];
    char tmtimes[40];
    time_t tctime;
    time_t tmtime;

    if (!stat) {
        PRINTF("null\n");
        return;
    }
    tctime = stat->ctime/1000;
    tmtime = stat->mtime/1000;

    ctime_r(&tmtime, tmtimes);
    ctime_r(&tctime, tctimes);

    PRINTF("\tctime = %s\tczxid=%llx\n"
    "\tmtime=%s\tmzxid=%llx\n"
    "\tversion=%x\taversion=%x\n"
    "\tephemeralOwner = %llx\n",
     tctimes, (long)stat->czxid, tmtimes,
    (long )stat->mzxid,
    (unsigned int)stat->version, (unsigned int)stat->aversion,
    (long) stat->ephemeralOwner);
}


static void dumpStringVec(const struct String_vector *strings){
   if (strings){
      PRINTF("stringvec count:%d value:{",strings->count);
      for (int i=0; i < strings->count; i++) {
        PRINTF("\t%s\n", strings->data[i]);
      } 
      PRINTF("}\n");   
   }
   else{
     PRINTF("stringvec is null\n");
   }
}


static int reportErr(const char *fmt,...){
    char* buf[256];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, 256, fmt, ap);
    va_end(ap);

    PRINTF(buf);
}

static int getMode(int isTmp,int isSeq){
    if(isTmp){
      if(isSeq){
        return ZOO_EPHEMERAL_SEQUENTIAL;
      }
      else{
        return ZOO_EPHEMERAL;
      }
    }
    else{
      if(isSeq){
        return ZOO_PERSISTENT_SEQUENTIAL;
      }
      else{
        return ZOO_PERSISTENT;
      }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////监控函数/////////////////////////////////////////////////////////////////////////
static void globalWacherFn(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx){
    zkclient* cli = watcherCtx;
    PRINTF("trigger event type:%s state:%s ",type2String(type),state2String(state));
    if(type == ZOO_SESSION_EVENT){
        if(state == ZOO_NOTCONNECTED_STATE){
          //初始阶段socket fd都没创建
          zkclientDisconnect(cli);
        }
        else if(state == ZOO_CONNECTING_STATE){
          //TCP 三次握手中
          zkclientcConnecting(cli);
        }
        else if(state == ZOO_ASSOCIATING_STATE){
          //三次握手完成 未收到prime respond包(会话协商阶段)
          zkclientcConnecting(cli);
        }
        else if(state == ZOO_CONNECTED_STATE){    
          //建立会话
          zkclientcConnected(cli);
        }
        else if(state == ZOO_EXPIRED_SESSION_STATE){ // 
          //会话过期，不可恢复。被动关闭 回调逻辑层
          zkclientDisconnect(cli);
        }
        else if(state == ZOO_AUTH_FAILED_STATE){
          //验证失败,不可恢复。被动关闭 回调逻辑层
          zkclientDisconnect(cli);
        }
        else {
          PRINTF("invaild state:%s\n",state2String(state));
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





zkclient* zkclientCreate(sessionConnectedHandler connectHandle,sessionCloseHandler closeHandle){
    zkclient* cli =  malloc(sizeof(zkclient));
    assert(cli != 0);
    cli->zh = 0;
    cli->stop = 0;
    cli->connectHandle = connectHandle;
    cli->closeHandle = closeHandle;
    cli->sessionState = SESSION_STATE_CLOSED;
    cli->sessionExpireTimeout = 0;
    cli->connectingStartTime = 0;
    return cli;
}


void zkclientFree(zkclient* cli){
    assert(cli);
    zkclientDisconnect(cli);
    free(cli);
    cli = 0;
}

//建立会话
int zkclientConnect(zkclient* cli,const char* host){
    assert(cli);
    zkclient* zh = zookeeper_init(host, globalWacherFn, RECV_TIMEOUT, &(cli->myid), cli, 0);
    if (!zh) {
        return errno;
    }
    cli->zh = zh;
    return 0;
}

static void zkclientcConnected(zkclient* cli){
    assert(cli);
    const clientid_t * id = zoo_client_id(cli->zh);
    if(cli->myid.client_id == 0 ||  id->client_id != cli->myid.client_id){
        cli->myid = *id;
    }
    if( cli->connectHandle ){
      cli->connectHandle(cli);
    }

    cli->sessionExpireTimeout = zoo_recv_timeout(cli->zh);

    zkclientSetState(cli,SESSION_STATE_CONNECTED);
}

static void zkclientcConnecting(zkclient* cli){
    assert(cli);
    cli->connectingStartTime = OnGetCurrentMs();
    zkclientSetState(cli,SESSION_STATE_CONNECTING);
}


int zkclientDisconnect(zkclient* cli){
   assert(cli);
   if(cli->zh){
        zookeeper_close(cli->zh);
        cli->zh = 0;
    }
    if(cli->closeHandle){
      cli->closeHandle(cli);
    }
    cli->sessionExpireTimeout = 0;
    cli->connectingStartTime  = 0;
    zkclientSetState(cli,SESSION_STATE_CLOSED);
}




int zkclientRun(zkclient* cli){
    if(!cli->stop) {
      zkclientSelect(cli);
    }
}

int zkclientStop(zkclient* cli){
    assert(cli);
    cli->stop = 1;
}


int zkclientSetState(zkclient* cli,int state){
   assert(cli != 0);
   cli->sessionState = state;
}

int zkclientGetState(zkclient* cli){
   assert(cli != 0);
   return cli->sessionState;
}
int zkclientIsConnected(zkclient* cli){
  return cli->sessionState == SESSION_STATE_CONNECTED;
}


// 发包+收包回+异步调用结果回调/监测函数回调
static int zkclientSelect(zkclient* cli){
    assert(cli);
    zhandle_t* zh = cli->zh;
    fd_set rfds, wfds, efds;
    FD_ZERO(&rfds); FD_ZERO(&wfds);  FD_ZERO(&efds);
    
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
 


//创建节点
int zkclientCreateNode(zkclient* cli,const char* path,const char* data,int len,int isTmp,int isSeq,createNodeRTHandler watcher,void* context){
    assert(cli);

    CHECK_CONNECTED(cli)

    int mode = getMode( isTmp, isSeq);

    RtContext* rtCx = malloc(sizeof(RtContext));
    assert(rtCx);
    rtCx->cli = cli;
    memcpy(rtCx->path,path,(strlen(path) + 1) * sizeof(char) );
    rtCx->context = context;
    rtCx->createRTHandler = watcher;
    
    int rc = zoo_acreate(cli->zh,path,data,len,&ZOO_OPEN_ACL_UNSAFE,mode,createNodeCompletion,rtCx);

    CHECK_RC(rc,"create node")
   
    return 0;
 }

static void createNodeCompletion(int rc, const char *value, const void *data) {
    RtContext* rtCx = data;
    assert(rtCx);
    int code = 0;
    if(rc == ZOK){
      code = ZKRT_SUCCESS;
    }
    else if(rc == ZNONODE){  //父节点不存在
      code = ZKRT_NONODE;
    }
    else if(rc == ZNODEEXISTS){  //节点已存在
      code = ZKRT_NODEEXIST;
    }
    else{
      code = ZKRT_ERROR;
    }
    PRINTF("create node:%s  data:%s result:%s\n",rtCx->path,value,zerror(rc));
    if(rtCx->createRTHandler){
       rtCx->createRTHandler(rtCx->cli,code,rtCx->path,value,rtCx->context);
    }

    free(rtCx);
}


//设置数据
 int zkclientSetNode(zkclient* cli,const char* path,const char* buff,int bufflen,setNodeRTHandler watcher,void* context){
    assert(cli);
    CHECK_CONNECTED(cli)

    RtContext* rtCx = malloc(sizeof(RtContext));
    assert(rtCx);
    rtCx->cli = cli;
    memcpy(rtCx->path,path,(strlen(path) + 1) * sizeof(char) );
    rtCx->context = context;
    rtCx->setRTHandler = watcher;

    int rc = zoo_aset(cli->zh,path,buff,bufflen,-1,setDataCompletion,rtCx);

    CHECK_RC(rc,"set node")

    return 0;
 }



static  void setDataCompletion(int rc, const struct Stat *stat,const void *data)
{
    RtContext* rtCx = data;
    assert(rtCx);
    int code = 0;
    if(rc == ZOK){
      code = ZKRT_SUCCESS;
    }
    else if(rc == ZNONODE){  //节点不存在
      code = ZKRT_NONODE;
    }
    else{
      code = ZKRT_ERROR;
    }
    PRINTF("set node:%s  result:%s\n",rtCx->path,zerror(rc));
    dumpStat(stat);
    if(rtCx->setRTHandler){
       rtCx->setRTHandler(rtCx->cli,code,rtCx->path,stat,rtCx->context);
    }
    free(rtCx);
}

//获取数据
int zkclientGetNode(zkclient* cli,const char* path,getNodeRTHandler watcher,void* context){
    assert(cli);
    CHECK_CONNECTED(cli)

    RtContext* rtCx = malloc(sizeof(RtContext));
    assert(rtCx);
    rtCx->cli = cli;
    memcpy(rtCx->path,path,(strlen(path) + 1) * sizeof(char) );
    rtCx->context = context;
    rtCx->getRTHandler = watcher;

    int rc =  zoo_aget(cli->zh,path, 1,getDataCompletion, rtCx);
    CHECK_RC(rc,"get node")
    return 0;
 }



static void getDataCompletion(int rc, const char *value, int value_len,const struct Stat *stat, const void *data)
{
    RtContext* rtCx = data;
    assert(rtCx);
    int code = 0;
    if(rc == ZOK){
      code = ZKRT_SUCCESS;
    }
    else if(rc == ZNONODE){  //节点不存在
      code = ZKRT_NONODE;
    }
    else{
      code = ZKRT_ERROR;
    }
    PRINTF("get node:%s value:%s result:%s\n",rtCx->path,value,zerror(rc));
    dumpStat(stat);
    if(rtCx->getRTHandler){
       rtCx->getRTHandler(rtCx->cli,code,rtCx->path,value,value_len,stat,rtCx->context);
    }
    free(rtCx);
}



//删除节点
int zkclientDelNode(zkclient* cli,const char* path,deleteNodeRTHandler watcher,void* context){
    assert(cli);
    CHECK_CONNECTED(cli)

    RtContext* rtCx = malloc(sizeof(RtContext));
    assert(rtCx);
    rtCx->cli = cli;
    memcpy(rtCx->path,path,(strlen(path) + 1) * sizeof(char) );
    rtCx->context = context;
    rtCx->deleteRTHandler = watcher;

    int rc =  zoo_adelete(cli->zh,path, -1,deleteNodeCompletion, rtCx);
    CHECK_RC(rc,"delete node")

    return 0;
}

static void deleteNodeCompletion(int rc, const void *data){
    RtContext* rtCx = data;
    assert(rtCx);
    int code = 0;
    if(rc == ZOK){
      code = ZKRT_SUCCESS;
    }
    else if(rc == ZNONODE){  //节点不存在
      code = ZKRT_NONODE;
    }
    else{
      code = ZKRT_ERROR;
    }
    PRINTF("delete node:%s  result:%s\n",rtCx->path,zerror(rc));
    if(rtCx->deleteRTHandler){
       rtCx->deleteRTHandler(rtCx->cli,code,rtCx->path,rtCx->context);
    }
    free(rtCx);
}



//获取子节点列表
int zkclientGetChildrens(zkclient* cli,const char* path,getChildrenNodeRTHandler watcher,void* context){
    assert(cli);
    CHECK_CONNECTED(cli)

    RtContext* rtCx = malloc(sizeof(RtContext));
    assert(rtCx);
    rtCx->cli = cli;
    memcpy(rtCx->path,path,(strlen(path) + 1) * sizeof(char) );
    rtCx->context = context;
    rtCx->getChildrenRTHandler = watcher;

    return zkclientWGetChildrens(cli, rtCx,path,0);
}

static int zkclientWGetChildrens(zkclient* cli,RtContext* rtCx,char* path,watcher_fn fn ){
    int rc= zoo_awget_children(cli->zh, path, fn, rtCx, getChildrenCompletion, rtCx);
    CHECK_RC(rc,"zoo_aget_children")
    return 0;
}



static void getChildrenCompletion(int rc, const struct String_vector *strings,const void *data) {
    RtContext* rtCx = data;
    assert(rtCx);
    int code = 0;
    if(rc == ZOK){
      code = ZKRT_SUCCESS;
    }
    else if(rc == ZNONODE){  //节点不存在
      code = ZKRT_NONODE;
    }
    else{
      code = ZKRT_ERROR;
    }
    PRINTF("get childrens node:%s  result:%s\n",rtCx->path,zerror(rc));
    dumpStringVec(strings);
    if(rtCx->getChildrenRTHandler){
       rtCx->getChildrenRTHandler(rtCx->cli,code,rtCx->path,strings,rtCx->context);
    }
    if(rtCx->wacher ){
      if(rc != ZOK){
          rtCx->wacher(rtCx->cli, EventNodeFail,rtCx->path,rtCx->context);
          free(rtCx);
      }
    }
    else{
      free(rtCx);
    }
}

//节点是否存在
int  zkclientExistNode(zkclient* cli, const char* path,existNodeRTHandler watcher,void* context){
    assert(cli);
    CHECK_CONNECTED(cli)

    RtContext* rtCx = malloc(sizeof(RtContext));
    assert(rtCx);
    rtCx->cli = cli;
    memcpy(rtCx->path,path,(strlen(path) + 1) * sizeof(char) );
    rtCx->context = context;
    rtCx->existRTHandler = watcher;

    return zkclientWGetChildrens(rtCx->cli, rtCx,rtCx->path,0);
}

static int zkclientWExist(zkclient* cli,RtContext* rtCx,char* path,watcher_fn fn ,stat_completion_t dc){
    int rc= zoo_awexists(cli->zh, rtCx->path, rtCx->wacher, rtCx,existCompletion, rtCx);
    CHECK_RC(rc,"zoo_awexists ")
    return 0;
}


static void existCompletion(int rc, const struct Stat *stat,const void *data){
    RtContext* rtCx = data;
    assert(rtCx);
    PRINTF("exist node:%s  result:%s\n",rtCx->path,zerror(rc));
    dumpStat(stat);

    int code = 0;
    if(rc == ZOK){
      code = ZKRT_SUCCESS;
    }
    else if(rc == ZNONODE){  //节点不存在
      code = ZKRT_NONODE;
    }
    else{
      code = ZKRT_ERROR;
    }
    if(rtCx->existRTHandler){
       rtCx->existRTHandler(rtCx->cli,code,rtCx->path,stat,rtCx->context);
    }

    if(rc != ZOK ){
       rtCx->wacher(rtCx->cli, EventNodeSubFail,rtCx->path ,rtCx->context);
       free(rtCx);
    }

    if(rtCx->wacher ){
      if(rc != ZOK && rc != ZNONODE){
          rtCx->wacher(rtCx->cli, EventNodeFail,rtCx->path,rtCx->context);
          free(rtCx);
      }
    }
    else{
      free(rtCx);
    }

}




static int checkTriggerEvent(RtContext* rtCx,int type){ 
     if ( type == ZOO_CREATED_EVENT){
      if( rtCx->extr == EventNodeCreated ){
        return 1;
      }
     }
     else if ( type == ZOO_DELETED_EVENT){
      if( rtCx->extr == EventNodeDeleted ){
        return 1;
      }
     }
    else if ( type == ZOO_CHANGED_EVENT){
      if( rtCx->extr == EventNodeDataChanged ){
        return 1;
      }
     }
     
    else if ( type == ZOO_CHILD_EVENT){
      if( rtCx->extr == EventNodeChildrenChanged ){
        return 1;
      }
     }
     return 0;
}




static void eventWatcher(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx){
    RtContext* rtCx = watcherCtx;
    PRINTF("existWatcheFn type:%d \n",type);
    if( checkTriggerEvent(rtCx,type) ){
       rtCx->wacher(rtCx->cli, rtCx->extr,path,rtCx->context);
       free(rtCx);
    } 
    else{
      if( type == ZOO_NOTWATCHING_EVENT || type == ZOO_SESSION_EVENT){
         rtCx->wacher(rtCx->cli, EventNodeFail,path,rtCx->context);
         free(rtCx);
      }
      else{
        //不是我关注的事件 也不是出错事件  则重新注册
        onSubscribeEvent(rtCx);
      }
    }
 }



int zkclientSubscribeEvent(zkclient* cli,char* path,int eventType,nodeEventHandler wacher,void* context){
    assert(cli);
    CHECK_CONNECTED(cli)

    if(!wacher){
      return 0;
    }
    RtContext* rtCx = malloc(sizeof(RtContext));
    assert(rtCx);
    
    rtCx->cli = cli;
    memcpy(rtCx->path,path,(strlen(path) + 1) * sizeof(char) );
    rtCx->context = context;
    rtCx->wacher = wacher;
    rtCx->extr = eventType;

    return onSubscribeEvent( rtCx);
}


static int onSubscribeEvent(RtContext* rtCx){
   if(rtCx->extr == ZOO_CHILD_EVENT){
       return zkclientWGetChildrens(rtCx->cli, rtCx,rtCx->path,eventWatcher);
    }
    else{
       return zkclientWGetChildrens(rtCx->cli, rtCx,rtCx->path,eventWatcher);
    }
}


static int64_t OnGetCurrentMs() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

//主动检测会话超时
static int checkSessionExpired(zkclient* cli){
  assert(cli);
  if(cli->sessionExpireTimeout != 0){
      if( zkclientGetState(cli) == SESSION_STATE_CONNECTING){
        if(OnGetCurrentMs() - cli->connectingStartTime >  cli->sessionExpireTimeout){
          zkclientDisconnect(cli);
        }
      }
  }
}