#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "zkcli.h"







// void getChildrenRTHandler(zkclient* cli,int errCode,const char* path,const struct String_vector *strings,void* context){
//     printf("parentNode path:%s\n",path);
//     char childPath [256];
//     for(int i = 0;i < strings->count;++i){
//          sprintf(childPath,"%s/%s",path,strings->data[i]);
//          printf("childnode path:%s\n",childPath);
//          if( zkclientGetNode(cli,childPath,getRTHandler,cli)  < 0){

//          }
//     }
// }



// void createRTHandler(zkclient* cli,int errCode,const char* path,const char* value,void* context){
//     if(errCode == ZKRT_SUCCESS || errCode == ZKRT_NODEEXIST){
//         const char* parentPath = "/parent";
//         const char* childPath = "/parent/child";
//         //父节点创建成功 创建10个子节点
//         if(strcmp(parentPath,path) == 0){
//             for (size_t i = 0; i < 1; i++)
//             {
//                 // char* val[30];
//                 // sprintf(val,"this is child :%d",i) ;

//                 // zkclientSubscribeEvent(cli,parentPath,EventNodeChildrenChanged,nodeChangeEventHandler,cli);

//                 // zkclientCreateNode(cli,childPath,val,strlen(val) + 1,1,1,createRTHandler,0);
//                 // zkclientCreateNode(cli,childPath,val,strlen(val) + 1,1,0,createRTHandler,0);

//             }

//             zkclientGetChildrens(cli,parentPath,getChildrenRTHandler,cli);
//         }

//     }
// }


const char* parentPath = "/parent";
const char* childPath = "/parent/child";

void getRTHandler(zkclient* cli,int errCode,const char* path,const char* buff,int bufflen,const struct Stat *stat,void* context){
    printf("nodepath:%s data:%s errCode:%d\n",path,buff,errCode);
}

void parentNodeWacher(zkclient* cli,int event,const char* path,void* context){
    printf("nodeCreateEventHandler path:%s eventType:%s\n",path,Event2String(event) );


    zkclientGetNode(cli,parentPath,getRTHandler,cli);
    zkclientNodeWacher(cli,parentPath,parentNodeWacher,cli);
}


 void ConnectedHandler(zkclient* cli,int isReconnect){
    printf("ConnectedHandler \n");
    if(isReconnect){
        return ;
    }
  
    zkclientNodeWacher(cli,parentPath,parentNodeWacher,cli);
   

    zkclientCreateNode(cli,parentPath,"111",4,0,0,0,0);


    zkclientSetNode(cli,parentPath,"111",4,0,0);


    //zkclientDelNode(cli,parentPath,0,0);

    //test(cli);
    
   
    //zkclientSubscribeEvent(cli,parentPath,EventNodeChildrenChanged,nodeChildChangeEventHandler,cli);

    //zkclientCreateNode(cli,childPath,"111",4,1,0,0,0);
   
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
     