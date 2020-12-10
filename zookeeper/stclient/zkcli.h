/////////////////////单线程客户端///////////////////////////////////
#ifndef __ZKCLIENT_H__
#define __ZKCLIENT_H__

typedef struct zkclient zkclient;

zkclient* zkclient_create();
void zkclient_free(zkclient* cli);

int zkclient_connect(zkclient* cli,const char* host);
int zkclient_run(zkclient* cli);
int zkclient_stop(zkclient* cli);

int zkclient_is_connected(zkclient* cli);


#endif