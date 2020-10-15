#ifndef __CONNECTION_H__
#define __CONNECTION_H__


#define CON_INIT 1
#define CON_CONNECTING 2 
#define CON_CONNECTED 3



typedef struct connection
{
	int fd;
	char* sbuffer;     		  //send cache
	char* rbuffer;     		  //read cache
	int css;                  //cur send size
	int crs;                  //cur read size
	int state;                //fd state
}connection;


connection* createConnection(int fd);
void freeConnection(connection* con);
int setConnectState(connection* con,int state);
int isConnnecting(connection* con);

#endif