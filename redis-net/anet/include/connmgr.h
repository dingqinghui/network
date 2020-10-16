#ifndef __CONNMGR_H__
#define __CONNMGR_H__

#include "../include/connction.h"


typedef struct conmgr{
    int maxcon;
    connection** conlist;
}conmgr;

void        createConMgr(int maxcon);
void        addConnction(connection* con) ;
int         removeConnction(connection* con) ;
connection* getConnection(int fd);


#endif