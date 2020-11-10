#include "../include/netlogger.h"



struct nlog;
static struct nlog* log = 0;

int netloger_init(){
    log = nlog_init("./netlog",NLOG_LVL_DEBUG,1024*1024*50);
}


struct nlog* netloger_ins(){
    return log;
}



