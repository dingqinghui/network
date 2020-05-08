#ifndef __server_mgr_h__
#define __server_mgr_h__

#include "../net/include/net_server.h"
#include "net_callback.h"


typedef struct 
{

}server_mgr;


server_mgr* ser_mgr = NULL;

server_mgr* create_server_mgr(int maxfd) {

	ser_mgr = malloc(sizeof(server_mgr));
	if (!ser_mgr) {
		exit(1);
		return NULL;
	}

	create_connect_mgr(maxfd);
	net_server_create(maxfd, Connect, Accept, Recv, Close, Error);
}

void release_server_mgr() {
	if (!ser_mgr) {
		return;
	}
	net_server_release();
	release_connect_mgr();

	free(ser_mgr);
}

server_mgr* server_mgr_run() {
	while (1) {
		net_server_run();
	}
}

int server_mgr_lisent(char* IP, int port) {
	return net_server_lisent(IP, port);
}
int server_mgr_connect(char* IP, int port) {
	return net_server_connect(IP, port);
}

int server_mgr_send(int id, char* buffer, int size) {
	return net_server_send(id, buffer, size);
}



#endif