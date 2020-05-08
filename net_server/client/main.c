#include <stdio.h>
#include "../common/server_mgr.h"

int main()
{
	 
	create_server_mgr(1024);

	for (int index = 0; index < 100; ++index)
	{
		server_mgr_connect("127.0.0.1", 8888);
	}

	server_mgr_run();

	printf("退出进程");

	release_server_mgr();

    return 0;
}