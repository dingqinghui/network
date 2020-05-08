#include <stdio.h>
#include "../common/server_mgr.h"


int main()
{
	create_server_mgr(1024);

	server_mgr_lisent("192.168.2.100", 8888);

	server_mgr_run();

	printf("退出进程");

	release_server_mgr();

    return 0;
}