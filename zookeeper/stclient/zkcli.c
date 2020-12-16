#include<stdio.h>
#include <stdlib.h>
#include <string.h>


#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>
#include "zkcli.h"


#define  SESSION_DEF_TIMEOUT 10000

#define PRINTF printf

#define CHECK_RC(rc,str)  \
if(rc != ZOK){ \
    onReportErr("%s. rc:%s\n",str,zerror(rc));\
    return -1;\
}

#define CHECK_CONNECTED(cli) \
if(!zkclientIsConnected(cli)){ \
  PRINTF("error zkclient state is %d",zkclientGetState(cli));\
  return -1; \
}


static const char* onState2String(int state);
static const char* onType2String(int state);
static void onDumpStat(const struct Stat *stat);
static void onDumpStringVec(const struct String_vector *strings);
static int onReportErr(const char *fmt,...);
static int64_t OnGetCurrentMs() ;

static int onGetMode(int isTmp,int isSeq);
static void onSessionEventWacherFn(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx);
static void onConnected(zkclient* cli);
static void onConnecting(zkclient* cli);
static int onDisconnect(zkclient* cli,int isExpire);
static int onSetState(zkclient* cli,int state);
static int onSelect(zkclient* cli);
static int onCheckSessionExpired(zkclient* cli);
static int onTriggerWacher(RtContext* rtCx,int event);

static int onWExist(zkclient* cli,RtContext* rtCx,char* path,watcher_fn fn);
static int onWGetChildrens(zkclient* cli,RtContext* rtCx,char* path,watcher_fn fn );

static void onGetChildrenCompletion(int rc, const struct String_vector *strings,const void *data) ;
static void onDeleteNodeCompletion(int rc, const void *data);
static void onGetDataCompletion(int rc, const char *value, int value_len,const struct Stat *stat, const void *data);
static void onSetDataCompletion(int rc, const struct Stat *stat,const void *data);
static void onCreateNodeCompletion(int rc, const char *value, const void *data) ;
static void onExistCompletion(int rc, const struct Stat *stat,const void *data);

static void onEventWatcher(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx);
static int onGetEvent(RtContext* rtCx,int type);
static int onSubscribeEvent(RtContext* rtCx);


typedef struct zkclient{
    zhandle_t* zh;
    clientid_t myid;
    int stop;
    sessionConnectedHandler connectHandle;
    sessionCloseHandler closeHandle;
    int sessionState;
    int sessionExpireTimeout;
    int64_t connectingStartTime;
    int timeout;
    char* host;
}zkclient;


/////////////////////////////////////////////////////工具函数/////////////////////////////////////////////////////////////////////////

//会话状态
static const char* onState2String(int state){
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
static const char* onType2String(int state){
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

static void onDumpStat(const struct Stat *stat) {
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


static void onDumpStringVec(const struct String_vector *strings){
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


static int onReportErr(const char *fmt,...){
    char* buf[256];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, 256, fmt, ap);
    va_end(ap);

    PRINTF(buf);
}

static int onGetMode(int isTmp,int isSeq){
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



static int64_t OnGetCurrentMs() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////监控函数/////////////////////////////////////////////////////////////////////////
static void onSessionEventWacherFn(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx){
    zkclient* cli = watcherCtx;
    PRINTF("trigger event type:%s state:%s \n",onType2String(type),onState2String(state));
    if(type == ZOO_SESSION_EVENT){
        if(state == ZOO_NOTCONNECTED_STATE){
          //初始阶段socket fd都没创建
          onDisconnect(cli,1);
        }
        else if(state == ZOO_CONNECTING_STATE){
          //TCP 三次握手中
          onConnecting(cli);
        }
        else if(state == ZOO_ASSOCIATING_STATE){
          //三次握手完成 未收到prime respond包(会话协商阶段)
          onConnecting(cli);
        }
        else if(state == ZOO_CONNECTED_STATE){    
          //建立会话
          onConnected(cli);
        }
        else if(state == ZOO_EXPIRED_SESSION_STATE){ // 
          //会话过期，不可恢复。被动关闭 回调逻辑层
          onDisconnect(cli,1);
        }
        else if(state == ZOO_AUTH_FAILED_STATE){
          //验证失败,不可恢复。被动关闭 回调逻辑层
          onDisconnect(cli,1);
        }
        else {
          PRINTF("invaild state:%s\n",onState2String(state));
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





zkclient* zkclientCreate(const char* host,sessionConnectedHandler connectHandle,sessionCloseHandler closeHandle,int timeout){
    zkclient* cli =  malloc(sizeof(zkclient));
    assert(cli != 0);
    cli->zh = 0;
    cli->stop = 0;
    cli->connectHandle = connectHandle;
    cli->closeHandle = closeHandle;
    cli->sessionState = SESSION_STATE_CLOSED;
    cli->sessionExpireTimeout = 0;
    cli->connectingStartTime = 0;
    cli->timeout = timeout > 0 ? timeout : SESSION_DEF_TIMEOUT;
    cli->host = strdup(host);

    zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);

    return cli;
}


void zkclientFree(zkclient* cli){
    assert(cli);
    zkclientDisconnect(cli);
    if(cli->host){
      free(cli->host);
    }
    free(cli);
    cli = 0;
}

//建立会话
int zkclientConnect(zkclient* cli){
    assert(cli);
    zkclient* zh = zookeeper_init(cli->host, onSessionEventWacherFn, cli->timeout, &(cli->myid), cli, 0);
    if (!zh) {
        return errno;
    }
    cli->zh = zh;
    return 0;
}

static void onConnected(zkclient* cli){
    assert(cli);
    onSetState(cli,SESSION_STATE_CONNECTED);

    int isReconnect = 1;
    const clientid_t * id = zoo_client_id(cli->zh);
    if(cli->myid.client_id == 0 ||  id->client_id != cli->myid.client_id){
        cli->myid = *id;
        isReconnect = 0;
    }

    if( cli->connectHandle ){
      cli->connectHandle(cli,isReconnect);
    }

    cli->sessionExpireTimeout = zoo_recv_timeout(cli->zh);
}

static void onConnecting(zkclient* cli){
    assert(cli);
    onSetState(cli,SESSION_STATE_CONNECTING);
    cli->connectingStartTime = OnGetCurrentMs();
}

static int onDisconnect(zkclient* cli,int isExpire){
    assert(cli);
    if(zkclientGetState(cli) == SESSION_STATE_CLOSED){
      return 0;
    }

    onSetState(cli,SESSION_STATE_CLOSED);

    if(cli->zh){
        zookeeper_close(cli->zh);
        cli->zh = 0;
    }
    if(cli->closeHandle){
      cli->closeHandle(cli,isExpire);
    }
    cli->sessionExpireTimeout = 0;
    cli->connectingStartTime  = 0;

    memset(&(cli->myid),0,sizeof(clientid_t));
    
    return 0;
}


int zkclientDisconnect(zkclient* cli){
    return onDisconnect(cli,0);
}




int zkclientRun(zkclient* cli){
    if(!cli->stop) {
      onSelect(cli);
      onCheckSessionExpired(cli);
    }
}

int zkclientStop(zkclient* cli){
    assert(cli);
    cli->stop = 1;
}


static int onSetState(zkclient* cli,int state){
   assert(cli != 0);
   cli->sessionState = state;
}

int zkclientGetState(zkclient* cli){
   assert(cli != 0);
   return cli->sessionState;
}

int zkclientIsConnected(zkclient* cli){
  return zkclientGetState(cli) == SESSION_STATE_CONNECTED;
}


// 发包+收包回+异步调用结果回调/监测函数回调
static int onSelect(zkclient* cli){
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

    int mode = onGetMode( isTmp, isSeq);

    RtContext* rtCx = malloc(sizeof(RtContext));
    assert(rtCx);
    rtCx->cli = cli;
    memcpy(rtCx->path,path,(strlen(path) + 1) * sizeof(char) );
    rtCx->context = context;
    rtCx->createRTHandler = watcher;
    
    int rc = zoo_acreate(cli->zh,path,data,len,&ZOO_OPEN_ACL_UNSAFE,mode,onCreateNodeCompletion,rtCx);

    CHECK_RC(rc,"create node")
   
    return 0;
 }

static void onCreateNodeCompletion(int rc, const char *value, const void *data) {
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
    //PRINTF("create node:%s  data:%s result:%s\n",rtCx->path,value,zerror(rc));
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

    int rc = zoo_aset(cli->zh,path,buff,bufflen,-1,onSetDataCompletion,rtCx);

    CHECK_RC(rc,"set node")

  return 0;
 }



static  void onSetDataCompletion(int rc, const struct Stat *stat,const void *data)
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
    ///PRINTF("set node:%s  result:%s\n",rtCx->path,zerror(rc));
    onDumpStat(stat);
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

    int rc =  zoo_aget(cli->zh,path, 0,onGetDataCompletion, rtCx);
    CHECK_RC(rc,"get node")
    return 0;
 }



static void onGetDataCompletion(int rc, const char *value, int value_len,const struct Stat *stat, const void *data)
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
    //PRINTF("get node:%s value:%s result:%s\n",rtCx->path,value,zerror(rc));
    onDumpStat(stat);
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

    int rc =  zoo_adelete(cli->zh,path, -1,onDeleteNodeCompletion, rtCx);
    CHECK_RC(rc,"delete node")

    return 0;
}

static void onDeleteNodeCompletion(int rc, const void *data){
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

    return onWGetChildrens(cli, rtCx,path,0);
}

static int onWGetChildrens(zkclient* cli,RtContext* rtCx,char* path,watcher_fn fn ){
    int rc= zoo_awget_children(cli->zh, path, fn, rtCx, onGetChildrenCompletion, rtCx);
    CHECK_RC(rc,"zoo_aget_children")
    return 0;
}



static void onGetChildrenCompletion(int rc, const struct String_vector *strings,const void *data) {
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
    //PRINTF("get childrens node:%s  result:%s\n",rtCx->path,zerror(rc));
    onDumpStringVec(strings);
    if(rtCx->getChildrenRTHandler){
       rtCx->getChildrenRTHandler(rtCx->cli,code,rtCx->path,strings,rtCx->context);
    }
    if(rc != ZOK){
      onTriggerWacher(rtCx,EventWacherFail);
      free(rtCx);
    }
    else{
      rtCx->wacher == 0 ? free(rtCx) : 0 ;
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

    return onWExist(rtCx->cli, rtCx,rtCx->path,0);
}

static int onWExist(zkclient* cli,RtContext* rtCx,char* path,watcher_fn fn){
    
    int rc= zoo_awexists(cli->zh, rtCx->path, fn, rtCx,onExistCompletion, rtCx);
    CHECK_RC(rc,"zoo_awexists ")
    return 0;
}


static void onExistCompletion(int rc, const struct Stat *stat,const void *data){
    RtContext* rtCx = data;
    assert(rtCx);
    //PRINTF("exist node:%s  result:%s\n",rtCx->path,zerror(rc));
    //onDumpStat(stat);

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
    if(rc != ZOK && rc != ZNONODE){
      onTriggerWacher(rtCx,EventWacherFail);
      free(rtCx);
    }
    else{
      rtCx->wacher == 0 ? free(rtCx) : 0 ;
    }

}






int zkclientNodeWacher(zkclient* cli,char* path,nodeEventHandler wacher,void* context){
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
    rtCx->existRTHandler = 0;

    PRINTF("zkclientNodeWacher  rtCx:%x path:%s  \n",rtCx,rtCx->path);

    return onWExist(rtCx->cli, rtCx,rtCx->path,onEventWatcher);
}


int zkclientChildWacher(zkclient* cli,char* path,nodeEventHandler wacher,void* context){
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
    rtCx->getChildrenRTHandler = 0;

    PRINTF("zkclientChildWacher  rtCx:%x path:%s  \n",rtCx,rtCx->path);

    return onWGetChildrens(rtCx->cli, rtCx,rtCx->path,onEventWatcher);
}

static int onGetEvent(RtContext* rtCx,int type){ 
     if ( type == ZOO_CREATED_EVENT){
      return EventNodeCreated;
     }
     else if ( type == ZOO_DELETED_EVENT){
      return EventNodeDeleted;
     }
    else if ( type == ZOO_CHANGED_EVENT){
     return EventNodeDataChanged;
     }
     
    else if ( type == ZOO_CHILD_EVENT){
      return EventNodeChildrenChanged;
    }
    return EventWacherFail;
}

static void onEventWatcher(zhandle_t *zh, int type,int state, const char *path,void *watcherCtx){
    if( type == ZOO_SESSION_EVENT){
      return ;
    }

    RtContext* rtCx = watcherCtx;
    int event = onGetEvent(rtCx,type);
    if( event >= 0 )
    {
        onTriggerWacher(rtCx,event);
        free(rtCx);
    } 
    else{
      if( type == ZOO_NOTWATCHING_EVENT ){
         onTriggerWacher(rtCx,EventWacherFail);
         free(rtCx);
      }
    }
 }



static int onTriggerWacher(RtContext* rtCx,int event){
   if(rtCx->wacher){
      rtCx->wacher(rtCx->cli, event,rtCx->path,rtCx->context);
    }
    rtCx = 0;
}

//主动检测会话超时
static int onCheckSessionExpired(zkclient* cli){
  assert(cli);
  if(cli->sessionExpireTimeout != 0){
      if( zkclientGetState(cli) == SESSION_STATE_CONNECTING){
        if(OnGetCurrentMs() - cli->connectingStartTime >  cli->sessionExpireTimeout){
          onDisconnect(cli,1);
        }
      }
  }
}


const char* Event2String(int event){
    if ( event == EventNodeCreated){
      return "EventNodeCreated";
    }
    else if ( event == EventNodeDeleted){
      return "EventNodeDeleted";
    }
    else if ( event == EventNodeDataChanged){
      return "EventNodeDataChanged";
    }
    else if ( event == EventNodeChildrenChanged){
      return "EventNodeChildrenChanged";
    }
    else if( event == EventWacherFail){
      return "EventWacherFail"; 
    }
    else{
      return "unknown event";
    }
}