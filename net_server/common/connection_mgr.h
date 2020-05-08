#ifndef  __connection_mgr__h__
#define  __connection_mgr__h__

#include <stdlib.h>
#include "buffer.h"


/////////////////////////////////////////////////////////////connection///////////////////////////////////////////////////////////////////////////////////


typedef struct
{
	int id;
	st_buffer* buffer;

}st_connection;


st_connection* create_connect(int id) {
	st_connection* con = malloc(sizeof(st_connection));
	if (!con)
		return NULL;
	con->id = id;
	return con;
}
void release_connect(st_connection* con) {
	if (!con)
		return ;
	free(con);
}


int connection_write(st_connection* con, char* buffer, int size) {
	if (!con)
		return 0;
	return write_to_buffer(con->buffer, buffer, size);
}




/////////////////////////////////////////////////////////////st_connect_mgr///////////////////////////////////////////////////////////////
typedef struct
{
	st_connection** connect_list;
	int max_con;

}st_connect_mgr;



st_connect_mgr* con_mgr = NULL;
st_connect_mgr* create_connect_mgr(int max_con) {
	con_mgr = malloc(sizeof(st_connect_mgr));
	if (!con_mgr) {
		exit(0);
		return NULL;
	}
		
	con_mgr->connect_list = malloc(sizeof(st_connection*) * max_con);
	if (!con_mgr->connect_list) {
		free(con_mgr);
		exit(0);
		return NULL;
	}

	con_mgr->max_con = max_con;
	for (int index = 0; index < max_con; ++index){
		(con_mgr->connect_list)[index] = NULL;
	}

	return con_mgr;
}

void release_connect_mgr() {
	if (!con_mgr)
		return NULL;
	for (int index = 0;index < con_mgr->max_con;++index){
		st_connection* con = (con_mgr->connect_list)[index];
		release_connect(con);
	}
	free(con_mgr);
}

int add_connection(st_connection* con) {
	if (!con)
		return -1;
	if (!con_mgr->connect_list)
		return -1;
	int id = con->id;
	if (id < 0 || id > con_mgr->max_con)
		return -1;
	if ((con_mgr->connect_list)[id])
		return -1;

	(con_mgr->connect_list)[id] = con;
	return 0;
}	

st_connection*  get_connection(int id) {
	if (!con_mgr)
		return NULL;
	if (!con_mgr->connect_list)
		return NULL;
	return (con_mgr->connect_list)[id];
}

int connection_mgr_send(int id,char* buffer,int size) {
	st_connection* con = get_connection( id);
	if (!con) {
		return 0;
	}
	return server_mgr_send(id, buffer, size);
}

#endif