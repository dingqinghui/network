
#ifndef __NET_DAEMON__
#define __NET_DAEMON__

int daemon_init(const char* daemonfile);
void daemon_exit(const char* daemonfile);

#endif