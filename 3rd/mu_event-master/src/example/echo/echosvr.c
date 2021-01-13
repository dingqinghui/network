
#include <stdio.h>
 #include <sys/time.h>
#include "servermanager.h"
#include "connection.h"
#include "listener.h"
#include "ring_buffer.h"
#include "config.h"
#include "timer.h"

static long long beginTM = 0;
static long long totalRecv = 0;

static long long  getMillisecond(){
    long long  t = 0;
    struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (long long)tv.tv_sec * 1000;
	t += tv.tv_usec / 1000;
    return t;
}

void onMessage(connection *conn)
{
    int size = 0;
    char* msg = ring_buffer_get_msg(conn->ring_buffer_read, &size);
    //printf("read all : %s, %d\n", msg, size);

    char buf[] = "abcd";
    //connection_send(conn, buf, sizeof(buf) - 1);

    connection_send(conn, msg, size);

    totalRecv += size;
}

void onConnection(connection* conn)
{
    printf("connected!!! ip:port %s:%d\n", conn->client_ip, conn->client_port);
}

void onTimer(void* udata){
    printf("%llu %llu\n",getMillisecond(),beginTM);
	long long interval = getMillisecond() - beginTM;
	double recvM = totalRecv * 1000 / interval /(1024*1024);
	
	printf("recv :%f M/s \n",recvM);
    printf("recv :totalRecv:%f  interval:%llu %f M/s \n",totalRecv,interval,recvM);
}

int main(int argc, char* argv[])
{
    int port = DEFAULT_PORT;
    int thread_num = MAX_LOOP;
    if (argc >= 2)
        port = atoi(argv[1]);
    if (argc >= 3)
        thread_num = atoi(argv[2]);
  
    printf("port, thread_num is %d, %d \n", port, thread_num);

    timer ti;
    ti.type = TIMER_OPT_REPEAT;
    ti.time_out = 5000;
    ti.callback = onTimer;
    ti.arg = NULL;

   
    beginTM = getMillisecond();

	server_manager *manager = server_manager_create(port, thread_num);
	inet_address addr = addr_create("any", port);
	listener_create(manager, addr, onMessage, onConnection);

    server_manager_add_timer(manager, ti);

	server_manager_run(manager);

	return 0;
}


