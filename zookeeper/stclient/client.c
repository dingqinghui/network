#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "zkcli.h"




void createRTHandler(zkclient* cli,int errCode,const char* path,const char* value,void* context){
    if(errCode == ZKRT_SUCCESS || errCode == ZKRT_NODEEXIST){
        if(strcmp("/parent",path) == 0){
            for (size_t i = 0; i < 10; i++)
            {
                char* val[30];
                sprintf(val,"this is child :%d",i) ;
                zkclientCreateNode(cli,"/parent/child",val,strlen(val),1,1,createRTHandler,0);
            }
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
        const char* val = "this is parent";
        zkclientCreateNode(cli,"/parent",val,strlen(val),0,0,createRTHandler,0);
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
     