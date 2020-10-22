
#include <sys/types.h>         
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <string.h>

#include "../include/error.h"
#include "../include/netapi.h"

#define netSetError setError

int netClose(int fd){
	close(fd);
}


int netGetSocketErr(char* err,int fd) {
	int error;
	int len = sizeof(error);
	if( getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0){
		netSetError(err, "getsockopt: %s\n", strerror(errno));
		return NET_ERR;
	}
	return error;
}

int netIsNoblock(char* err,int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0) {
		netSetError(err, "fcntl(F_GETFL): %s\n", strerror(errno));
		return NET_ERR;
	}
	return flag | O_NONBLOCK;
}

int netSetNoblock(char* err,int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0) {
		netSetError(err, "fcntl(F_GETFL): %s\n", strerror(errno));
		return NET_ERR;
	}
	if( fcntl(fd, F_SETFL, flag | O_NONBLOCK) < 0){
		netSetError(err, "fcntl(F_SETFL:O_NONBLOCK): %s\n", strerror(errno));
		return NET_ERR;
	}
	return NET_OK;
}

//端口释放后可立刻被重用(bind) server应在bind之前设定此选项
int netSetTcpReuse(char* err,int fd) {
	int reuse = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		netSetError(err, "setsockopt(SOL_SOCKET:SO_REUSEADDR): %s\n", strerror(errno));
		return NET_ERR;
	}
	return NET_OK;
}

int netTcpNoDelay(char* err, int fd)
{
    int yes = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) < 0)
    {
        netSetError(err, "setsockopt(IPPROTO_TCP:TCP_NODELAY): %s\n", strerror(errno));
		return NET_ERR;
    }
    return NET_OK;
}


int netTcpServer(char*err,char* ip, int port)
{
	int lfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (lfd < 0) {
		netSetError(err, "socket: %s\n", strerror(errno));
		return NET_ERR;
	}

	if (netSetNoblock(err,lfd) < 0){
		close(lfd);
		return NET_ERR;
	}

	if (netSetTcpReuse(err,lfd) < 0 ) {
		close(lfd);
		return NET_ERR;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	if (bind(lfd, (struct sockaddr*) & addr, sizeof(addr)) < 0) {
		close(lfd);
		netSetError(err, "bind: %s\n", strerror(errno));
		return NET_ERR;
	}

	if (listen(lfd, 500) < 0) {
		close(lfd);
		netSetError(err, "listen: %s\n", strerror(errno));
		return NET_ERR;
	}
	return lfd;
}


int netTcpCommonConnect(char* err,char* ip, int port,int flags)
{
	int cfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (cfd < 0) {
		netSetError(err, "socket: %s\n", strerror(errno));
		return NET_ERR;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	if (flags & NET_SK_NOBLOCK != 0) 
    {
		if (netSetNoblock(err,cfd) == NET_ERR) {
			close(cfd);
			return NET_ERR;
		}
		if (connect(cfd, (struct sockaddr_in*) & addr, sizeof(addr)) < 0) {
			if (errno != EINPROGRESS) {
				close(cfd);
				netSetError(err, "connect: %s\n", strerror(errno));
				return NET_ERR;
			}
		}
		else {
			if (connect(cfd, (struct sockaddr_in*) & addr, sizeof(addr)) < 0) {
				close(cfd);
				netSetError(err, "connect: %s\n", strerror(errno));
				return NET_ERR;
			}
		}
	}
	return cfd;
}

int netTcpBlockConnect(char* err,char* ip, int port) {
	return netTcpCommonConnect(err,ip, port,NET_SK_BLOCK);
}


int netTcpNoBlockConnect(char* err,char* ip, int port) {
	return netTcpCommonConnect(err,ip, port, NET_SK_NOBLOCK);
}

int netRead(int fd,void* buf,int size){
	return read(fd,buf,size);
}

int netWrite(int fd,void* buf,int size){
	return write(fd,buf,size);
}

//fd io write/read error
int netIoError(char* err,int fd){
	if(netIsNoblock(err,fd)){
		if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR){
			return NET_OK;
		}
	}
	return NET_ERR;
}

int netAccept(char* err,int lfd) {
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	int fd = accept(lfd, (struct sockaddr*) & remoteAddr, &nAddrlen);
	if (fd <= 0) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			netSetError(err, "accept: %s\n", strerror(errno));
			return NET_ERR;
		}
	}
	return fd;
}