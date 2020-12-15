#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "zkcli.h"


void nodeChangeEventHandler(zkclient* cli,int eventType,const char* path,void* context){
    printf("nodeChangeEventHandler path:%s eventType:%d\n",path,eventType);
}


void getRTHandler(zkclient* cli,int errCode,const char* path,const char* buff,int bufflen,const struct Stat *stat,void* context){

    printf("nodepath:%s data:%s\n",path,buff);
}

void getChildrenRTHandler(zkclient* cli,int errCode,const char* path,const struct String_vector *strings,void* context){
    printf("parentNode path:%s\n",path);
    char childPath [256];
    for(int i = 0;i < strings->count;++i){
         sprintf(childPath,"%s/%s",path,strings->data[i]);
         printf("childnode path:%s\n",childPath);
         if( zkclientGetNode(cli,childPath,getRTHandler,cli)  < 0){

         }
    }
}



void createRTHandler(zkclient* cli,int errCode,const char* path,const char* value,void* context){
    if(errCode == ZKRT_SUCCESS || errCode == ZKRT_NODEEXIST){
        const char* parentPath = "/parent";
        const char* childPath = "/parent/child";
        //父节点创建成功 创建10个子节点
        if(strcmp(parentPath,path) == 0){
            for (size_t i = 0; i < 1; i++)
            {
                // char* val[30];
                // sprintf(val,"this is child :%d",i) ;

                // zkclientSubscribeEvent(cli,parentPath,EventNodeChildrenChanged,nodeChangeEventHandler,cli);

                // zkclientCreateNode(cli,childPath,val,strlen(val) + 1,1,1,createRTHandler,0);
                // zkclientCreateNode(cli,childPath,val,strlen(val) + 1,1,0,createRTHandler,0);

            }

            zkclientGetChildrens(cli,parentPath,getChildrenRTHandler,cli);
        }

    }
}


void sessionHandler(zkclient* cli,void* context){
    if(  zkclientIsExpired(cli) ){
        printf("session expire \n");
        return ;
    }
    if( zkclientIsConnected(cli) ){
        printf("session connected \n");
        //const char* val = "this is parent";
        //zkclientCreateNode(cli,"/parent",val,strlen(val) + 1,0,0,createRTHandler,0);
        const char* parentPath = "/parent";
        const char* childPath = "/parent/child";
        //zkclientSubscribeEvent(cli,parentPath,EventNodeChildrenChanged,nodeChangeEventHandler,cli);

        //zkclientCreateNode(cli,childPath,"111",4,1,0,0,0);

       //zkclientSynCreateNode(cli,parentPath);

        return ;
    }
    else{
         printf("session error \n");
    }
}





int main(int argc,const char*argv[])
{

    char*host="127.0.0.1:2183,127.0.0.1:2184,127.0.0.1:2185";

    zkclient* zkcli  = zkclientCreate(sessionHandler,0);
    assert(zkcli);

    zkclientConnect(zkcli,host);

    

    while(1){
        zkclientRun(zkcli);
    }

    zkclientFree(zkcli);

    return 0;
}
     