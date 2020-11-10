
#include <stdio.h>

#include "logger.h"

/*
不支持多线程
10000       0.061s  
100000      0.466s
1000000     4.687s
*/


struct nlog;
int main(int argc,char* argv[])
{
    int cnt = 10000;
    char* format = "111111111111111111111111111111111111111111111111111111111111111111111111111111111111 %d\n";
    if(argc >= 2){
        cnt =  atoi(argv[1]);
        
    }
    if(argc >= 3){
        format = argv[2];
    }

	struct nlog* log = nlog_init("./game",NLOG_LVL_DEBUG,1024*1024);

    for(int i = 0;i < cnt;++i){
        LOG_DEBUG(log,format,i);
        LOG_DEBUG(log,"1111111 %d %d",12,34);
        //nlog_format2(log,NLOG_LVL_DEBUG,format,i);
    }

    nlog_free(log);
    return 0;
}