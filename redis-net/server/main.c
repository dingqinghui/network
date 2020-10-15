
#include <stdio.h>
#include "../anet/include/netserver.h"



int main()
{
	createServer(0);
	runServer(EV_WAIT_FPS);

    return 0;
}