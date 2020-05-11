#include <sys/time.h>
#include <stdio.h>
#include "../common/server_mgr.h"

int main()
{

	create_server_mgr(1024);


	for (int index = 0; index < 1000; ++index)
	{
		server_mgr_connect("192.168.2.100", 8888);
	}

	server_mgr_run();

	printf("退出进程");

	release_server_mgr();

    return 0;
}