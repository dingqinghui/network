
#include <stdio.h>
#include "../anet/include/netserver.h"
#include "../anet/include/netapi.h"


int main()
{

	createServer(0);
	for (size_t i = 0; i < 50; i++)
	{

		if( driveConnect("127.0.0.1", 8001,1) > 0)
		{
			//netWrite();
			printf(".........%d\n",i);
		}
		
	}
	
	runLoop(EV_WAIT_FPS) ;


    return 0;
}