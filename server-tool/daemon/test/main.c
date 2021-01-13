
#include   <stdio.h>
#include "../net_daemon.h"

int main(void){

  if( daemon_init("./test.pid") ){
    return 0;
  }
  while(1){

  }

  daemon_exit("./test.pid");
  return 0;
}