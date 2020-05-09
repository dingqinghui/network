#include <stdio.h>
#include "../common/server_mgr.h"

#include <time.h>

void Timer_Callback(void* udata) {
	int index = (int)udata;
	printf("index:%d curtime:%d \n", index,time(NULL));
}

int main()
{
	create_server_mgr(1024);


	for (int index = 1; index <= 10; ++index)
	{
		heap_timer* timer = create_timer(index + index * 2, Timer_Callback,index);
		if (timer)
			h_t_mgr_add_timer(timer);
	}


	server_mgr_lisent("192.168.2.100", 8888);

	server_mgr_run();

	printf("退出进程");

	release_server_mgr();

    return 0;
}