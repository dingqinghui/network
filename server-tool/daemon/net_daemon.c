
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include<sys/file.h>
#include "net_daemon.h"


int check_pid(const char* daemonfile){
   FILE * file = fopen(daemonfile,"r");
   if(!file){
      return 0;
   }
   int pid = 0;
   int n = fscanf(file,"%d",&pid);
   if(n != 1 || getpid() == pid || pid == 0 ){
      return 0;
   }
   //check pid process is  exist .
   if (kill(pid, 0) && errno == ESRCH)
		return 0;

   return pid;
}
int write_pid(const char* daemonfile){
   FILE * file = fopen(daemonfile,"w+");
   if(!file){
      printf("fopen(%s) fail.errno:%d\n",daemonfile,errno);
      return -1;
   }
   int fd =fileno(file);
   int pid = getpid();
   if(flock(fd,LOCK_EX | LOCK_NB  )){
      printf("flock(%s) fail.errno:%d\n",daemonfile,errno);
      fclose(file);
      return -1;
   }

   fprintf(file,"%d",pid);
   fflush(file);
   flock(fd,LOCK_UN);
   fclose(file);
   return 0;
}

int redirect_fd(){
   int fd = open("/dev/null",O_RDWR);
   if(fd < 0){
      printf("open /dev/null fail.errno:%d\n",errno);
      return -1;
   }

   if(dup2(fd,0) < 0){
      printf("redirect stdin.errno:%d\n",errno);
      return -1;
   }

   if(dup2(fd,1) < 0){
      printf("redirect stdout.errno:%d\n",errno);
      return -1;
   }

   if(dup2(fd,2) < 0){
      printf("redirect stderr.errno:%d\n",errno);
      return -1;
   }
   return 0;
}



int daemon_init(const char* daemonfile){
   int pid = check_pid( daemonfile );
   if(pid){
      printf("daemon already running pid:%d\n",pid);
      return -1;
   }

   if(daemon(1,1)){
      printf("daemon fail.errno:%d\n",errno);
      return -1;
   }
   
   if(write_pid(daemonfile)) {
      return -1;
   }

   if( redirect_fd() ){
      return -1;
   }
   return 0;
}


void daemon_exit(const char* daemonfile){
   unlink(daemonfile);
}