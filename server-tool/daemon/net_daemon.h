
#ifndef __NET_DAEMON__
#define __NET_DAEMON__
/*
   注意调用daemon_init前需要屏蔽SIGUP信号，否则daemon 收到SIGUP信号会退出
*/

int daemon_init(const char* daemonfile);
void daemon_exit(const char* daemonfile);

#endif