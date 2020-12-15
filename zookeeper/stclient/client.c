#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "zkcli.h"





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




void nodeCreateEventHandler(zkclient* cli,int eventType,const char* path,void* context){
    printf("nodeCreateEventHandler path:%s eventType:%d\n",path,eventType);
}

void nodeDeleteEventHandler(zkclient* cli,int eventType,const char* path,void* context){
    printf("nodeDeleteEventHandler path:%s eventType:%d\n",path,eventType);
}


void nodeChangeEventHandler(zkclient* cli,int eventType,const char* path,void* context){
    printf("nodeChangeEventHandler path:%s eventType:%d\n",path,eventType);
}


 void ConnectedHandler(zkclient* cli){
    printf("ConnectedHandler \n");

    const char* parentPath = "/parent";
    const char* childPath = "/parent/child";
    //zkclientSubscribeEvent(cli,parentPath,EventNodeCreated,nodeCreateEventHandler,cli);
   

    zkclientCreateNode(cli,parentPath,"111",4,0,0,0,0);


    //zkclientSubscribeEvent(cli,parentPath,EventNodeDeleted,nodeDeleteEventHandler,cli);
    //zkclientSubscribeEvent(cli,parentPath,EventNodeDataChanged,nodeChangeEventHandler,cli);

    zkclientSubscribeEvent(cli,parentPath,EventNodeChildrenChanged,nodeChangeEventHandler,cli);

    zkclientCreateNode(cli,childPath,"111",4,1,0,0,0);

    //zkclientSetNode(cli,parentPath,"111",4,0,0);
    //zkclientDelNode(cli,parentPath,0,0);

 }


 
 void CloseHandler(zkclient* cli,int isExpire){
    printf("CloseHandler \n");
    if(isExpire){
        zkclientConnect(cli);
    }
 }


int main(int argc,const char*argv[])
{

    char*host="127.0.0.1:2183,127.0.0.1:2184,127.0.0.1:2185";

    zkclient* zkcli  = zkclientCreate(host,ConnectedHandler,CloseHandler,0);
    assert(zkcli);

    zkclientConnect(zkcli);

    

    while(1){
        zkclientRun(zkcli);
    }

    zkclientFree(zkcli);

    return 0;
}
     