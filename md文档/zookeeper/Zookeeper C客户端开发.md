## 一.下载源码编译

### 1.下载git地址：

https://github.com/apache/zookeeper

### 2.编译：

在源码的根目录下执行 ant compile_jute

会在/zookeeper-client/zookeeper-client-c 下生成 generated文件夹

切换到/zookeeper-client/zookeeper-client-c目录执行 cmake cmake .

### 3.生成文件位置

库目录：

`/root/apache-zookeeper-3.5.8/zookeeper-client/zookeeper-client-c/.libs`

头文件目录：

`/usr/local/include/zookeeper`

### 4.项目引用

#### 1.头文件：

`#include <zookeeper/zookeeper.h>`

#### 2.编译选项

`-DTHREAD -lzookeeper_mt -lpthread -lm`

####  3.cmakefile  文件：

```SET(EXECUTABLE_OUTPUT_PATH ./bin)       #设置可执行文件的输出目录
SET(LIBRARY_OUTPUT_PATH .//lib)             #设置库文件的输出目录
set(CMAKE_BUILD_TYPE "Debug")

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
add_definitions(-g)
add_definitions(-DTHREAD)

link_directories(./lib)
include_directories(./common/zookeeper)

project (client)

# 指定生成目标
add_executable(client   client.c )
target_link_libraries(client pthread)
target_link_libraries(client zookeeper_mt)
target_link_libraries(client m)
```
#### 4.测试代码
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zookeeper.h>
#include <zookeeper_log.h>


void zktest_watcher_g(zhandle_t*zh,int type,int state,
const char*path,void*watcherCtx)
{
    printf("Somethinghappened.\n");
    printf("type:%d\n",type);
    printf("state:%d\n",state);
    printf("path:%s\n",path);
    printf("watcherCtx:%s\n",(char*)watcherCtx);
}
void zktest_dump_stat(const struct Stat*stat)
{
    char    tctimes[40];
    char    tmtimes[40];
    time_t tctime;
    time_t tmtime;
    if(!stat){
        fprintf(stderr,"null\n");
        return;
    }
    tctime=stat->ctime/1000;
    tmtime=stat->mtime/1000;

    ctime_r(&tmtime,tmtimes);
    ctime_r(&tctime,tctimes);

    fprintf(stderr,"\tctime=%s\tczxid=%llx\n"
        "\tmtime=%s\tmzxid=%llx\n"
        "\tversion=%x\taversion=%x\n"
        "\tephemeralOwner=%llx\n",
        tctimes,stat->czxid,
        tmtimes,stat->mzxid,
        (unsigned int)stat->version,(unsigned int)stat->aversion,
        stat->ephemeralOwner);
}

//当zoo_aexists请求完成时会调用该函数
//rc参数为:ZOK操作完成；ZNONODE节点不存在；ZNOAUTH客户端没有权限�除节点。
void zktest_stat_completion(int rc,const struct Stat*stat,const void*data)
{
    fprintf(stderr,"%s:rc=%dStat:\n",(char*)data,rc);
    zktest_dump_stat(stat);
}
void zktest_void_completion(int rc,const void*data)
{
    fprintf(stderr,"[%s]:rc=%d\n",(char*)(data==0?"null":data),rc);
}
//当创建节点请求完成时会调用该函数
//rc参数为:ZOK操作完成；ZNONODE父节点不存在；ZNODEEXISTS节点已存在；ZNOAUTH客户端没有权限创建节点。ZNOCHILDRENFOREPHEMERALS临时节点不能创建子节点
//value参数即新节点的路径名
//string_completion_tcompletion中constvoid*data参数即为zoo_acreate中的constvoid*data。
void zktest_string_completion(int rc,const char*name,const void*data)
{
    fprintf(stderr,"[%s]:rc=%d\n",(char*)(data==0?"null":data),rc);
    if(!rc){
        fprintf(stderr,"\tname=%s\n",name);
    }
}
int main(int argc,const char*argv[])
{
    const char*host="127.0.0.1:2181";
    //如果30秒内客户端没有连接上Zookeeper服务则表示连接超时。
    int timeout=3000;
    //设置日志等级。
    zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
    //初始化zookeeper句柄（zhandle_t）
    //第四个参数为客户端会话id,客户端尝试重连的先前会话的ID，如果不需要重连先前的会话，则设置为0
    //第五个参数为当前zk的上下文对象，最后一个参数是预留参数
    zhandle_t*zkhandle=zookeeper_init(host,
    zktest_watcher_g,timeout,0,"hellozookeeper.",0);
    if(zkhandle==NULL){
        fprintf(stderr,"Errorwhenconnectingtozookeeperservers...\n");
        exit(EXIT_FAILURE);
    }
    //structACLALL_ACL[]={{ZOO_PERM_ALL,ZOO_ANYONE_ID_UNSAFE}};
    //structACL_vectorALL_PERMS={1,ALL_ACL};
    //创建一个znode节点，节点路径为"/xyz",保存一个长度为5，内容为hello的数据，任何人都可以访问
    //这个一个异步函数，调用之后，直接往下执行，但是此时节点并没有真正的创建成功。节点创建成功之后会调用zktest_string_compeltion函数
    //最后一个参数是该回调函数，要�入的参数
    int ret=zoo_acreate(zkhandle,"/xyz","hello",5,
    &ZOO_OPEN_ACL_UNSAFE,0/*ZOO_SEQUENCE*/,
    zktest_string_completion,"acreate");

    if(ret){
        fprintf(stderr,"Error%dfor%s\n",ret,"acreate");
        exit(EXIT_FAILURE);
    }
    ret=0;

    //第三个参数是监视，如果非0，则在服务器端设置监视，当节点发生变化时客户端会得到通知，即使当前指定的节点不存在也会设置监视，这�该节点被创建时，客户端也可以得到通知。
    ret=zoo_aexists(zkhandle,"/xyz",1,zktest_stat_completion,"aexists");
    if(ret){
        fprintf(stderr,"Error%dfor%s\n",ret,"aexists");
        exit(EXIT_FAILURE);
    }
    ret=0;
    //Waitforasynchronouszookeepercalldone.
    getchar();
    //第三个参数:期望的节点版本号，如果真实的版本号与期望的版本号不同则zoo_delete()调用失败，-1表示不不检查版本号。
    ret=zoo_adelete(zkhandle,"/xyz",-1,zktest_void_completion,"adelete");
    if(ret){
        fprintf(stderr,"Error%dfor%s\n",ret,"adelete");
        exit(EXIT_FAILURE);
    }
    //Waitforasynchronouszookeepercalldone.
    getchar();
    zookeeper_close(zkhandle);
    return 0;
}
```



## 二.客户端开发注意问题

1.节点不可递归删除创建

2.重连后会重新注册已经激活的监控点，会根据客户端和服务端的版本变化马上对比，如果有差异马上会通知，这一点对用户完全透明。

3.客户端创建临时节点后。客户端挂掉，服务器等待超时后才删除临时节点。

4.同步API只有在多线程下才可以使用

5.exist 接口 当response中返回zok 或 znonode 时都会激活监视点

6.客户端不会主动监测会话超时。服务器关闭，客户端会一直重连，直到连接到服务器，才可能收到会话过期事件。客户端需要加一个检测，当从连接状态转变成连接中状态。则开始定时检测，是否会话超时。如果之前还未连接到服务器，没有会话也就不存在会话超时。

## 三.会话事件

![zookeeper 状态图](I:\a桌面\zookeeper 状态图.png)



## 参考文章

API：https://www.cnblogs.com/haippy/archive/2013/02/21/2920280.html

事件列表：https://blog.csdn.net/liu857279611/article/details/70495413

封装库：https://github.com/pingzilao/zookeeper_client_design_manual

常见问题：https://blog.csdn.net/kobejayandy/article/details/41791399

权限：https://zhuanlan.zhihu.com/p/54067428







