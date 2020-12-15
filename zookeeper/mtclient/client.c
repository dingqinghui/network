#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "zkcli.h"

#include <pthread.h>

      

void session_wacher(zkclient* cli){
    pthread_t id = pthread_self();
    if( zkclient_is_expire() ){
        printf("session expire %lu.\n",id);
    }
    else if( zkclient_is_connected() ){
        printf("session connected %lu.\n",id);
    }
    else{
        printf("session connecting %lu.\n",id);
    }
    struct timeval begintv = {0,0};
    gettimeofday(&begintv,0);
    for (size_t i = 0; i < 10000; i++)
    {
        static cnt = 0;
        zkclientSynExistNode(cli,"/synparent");
        //printf("count:%d \n",++cnt);
    } 

    struct timeval nowtv = {0,0};
    gettimeofday(&nowtv,0);

    printf("second:%d usecond:%d\n",nowtv.tv_sec -  begintv.tv_sec,nowtv.tv_usec -  begintv.tv_usec );
}


void create_wacher(struct wacher_context* wcc){
    assert(wcc);
    printf("path:%s rc:%s rvalue:%s expored:%d connected:%d \n",
        wcc->path,
        zerror(wcc->rc) ,
        wcc->rvalue,
        wcc->cli->expired,
        wcc->cli->connected);
}

int main(int argc,const char*argv[])
{
    pthread_t id = pthread_self();
    printf("main thread id:%lu\n",id);
    char*host="127.0.0.1:2183,127.0.0.1:2184,127.0.0.1:2185";

    zkclient* zkcli  = zkclient_create();
    assert(zkcli);
    if( zkclient_init(zkcli ,host,15000,session_wacher) < 0){

    }

    int value = 0;
    if( zkclient_create_node(zkcli,"/test",&value,sizeof(value),create_wacher,0) < 0){

    }

    while(1){
        sleep(1);
    }
    zkclient_free(zkcli);

    

    return 0;
}
     