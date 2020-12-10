#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "zkcli.h"




int main(int argc,const char*argv[])
{

    char*host="127.0.0.1:2183,127.0.0.1:2184,127.0.0.1:2185";

    zkclient* zkcli  = zkclient_create();
    assert(zkcli);

    zkclient_connect(zkcli,host);

    while(1){
        zkclient_run(zkcli);
    }

    zkclient_free(zkcli);

    return 0;
}
     