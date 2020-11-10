
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>


#include "../include/error.h"
#include "../include/netapi.h"

#define netSetError setError

int netClose(char* err,int fd){
	if( close(fd) < 0){
		netSetError(err, "close: fd:%d %s\n", fd,strerror(errno));
		return NET_ERR;
	}
}

int netSetRecvBuf(char* err,int fd,int size){
	int len = sizeof(size);
	if( setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, len) < 0){
		netSetError(err, "getsockopt(SO_RCVBUF): fd:%d  %s\n",fd, strerror(errno));
		return NET_ERR;
	}
	return NET_OK;
}


int netSetSendBuf(char* err,int fd,int size){
	int len = sizeof(size);
	if( setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, len) < 0){
		netSetError(err, "setsockopt(SO_SNDBUF): fd:%d  %s\n",fd, strerror(errno));
		return NET_ERR;
	}
	return NET_OK;
}

int netGetSocketErr(char* err,int fd) {
	int error;
	int len = sizeof(error);
	if( getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0){
		netSetError(err, "getsockopt: fd:%d  %s\n",fd, strerror(errno));
		return NET_ERR;
	}
	return error;
}

int netIsNoblock(char* err,int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0) {
		netSetError(err, "fcntl(F_GETFL):fd:%d  %s\n",fd, strerror(errno));
		return NET_ERR;
	}
	return flag | O_NONBLOCK;
}

int netSetNoblock(char* err,int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0) {
		netSetError(err, "fcntl(F_GETFL):fd:%d %s\n", fd,strerror(errno));
		return NET_ERR;
	}
	if( fcntl(fd, F_SETFL, flag | O_NONBLOCK) < 0){
		netSetError(err, "fcntl(F_SETFL:O_NONBLOCK): fd:%d %s\n",fd, strerror(errno));
		return NET_ERR;
	}
	return NET_OK;
}

//端口释放后可立刻被重用(bind) server应在bind之前设定此选项
int netSetTcpReuse(char* err,int fd) {
	int reuse = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		netSetError(err, "setsockopt(SOL_SOCKET:SO_REUSEADDR): fd:%d  %s\n", fd,strerror(errno));
		return NET_ERR;
	}
	return NET_OK;
}

int netTcpNoDelay(char* err, int fd)
{
    int yes = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) < 0)
    {
        netSetError(err, "setsockopt(IPPROTO_TCP:TCP_NODELAY): fd:%d %s\n",fd, strerror(errno));
		return NET_ERR;
    }
    return NET_OK;
}


int netTcpServer(char*err,char* ip, int port)
{
	int lfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (lfd < 0) {
		netSetError(err, "socket: fd:%d %s\n", strerror(errno));
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
		netSetError(err, "bind:fd: fd%d %s\n",lfd, strerror(errno));
		return NET_ERR;
	}

	if (listen(lfd, 500) < 0) {
		close(lfd);
		netSetError(err, "listen:  fd%d ,%s\n",lfd, strerror(errno));
		return NET_ERR;
	}
	return lfd;
}


int netTcpCommonConnect(char* err,char* ip, int port,int flags)
{
	int cfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (cfd < 0) {
		netSetError(err, "socket: fd%d ,%s\n", cfd,strerror(errno));
		return NET_ERR;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	if (flags ==  NET_SK_NOBLOCK ) 
    {
		if (netSetNoblock(err,cfd) == NET_ERR) {
			close(cfd);
			return NET_ERR;
		}
		if (connect(cfd, (struct sockaddr_in*) & addr, sizeof(addr)) < 0) {
			if (errno != EINPROGRESS) {
				close(cfd);
				netSetError(err, "connect: fd%d %s\n", cfd,strerror(errno));
				return NET_ERR;
			}
		}
	}
	else {
		if (connect(cfd, (struct sockaddr_in*) & addr, sizeof(addr)) < 0) {
			close(cfd);
			netSetError(err, "connect: fd%d  %s\n", cfd,strerror(errno));
			return NET_ERR;
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
	return  write(fd,buf,size);
}

//fd io write/read error
int netIoError(char* err,int fd){
	if(netIsNoblock(err,fd)){
		if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR){
			return NET_RET_OK;
		}
	}
	err = strerror(errno);
	return NET_RET_ERROR;
}

int netAcceptError(char* err){
	if(errno == EAGAIN  || errno == EINTR){ 
		return NET_RET_OK;
	}
	err = strerror(errno);
	return NET_RET_ERROR;
}

int netAccept(char* err,int lfd) {
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	int fd = accept(lfd, (struct sockaddr*) & remoteAddr, &nAddrlen);
	return fd;
}


struct sockaddr_in6 netGetLocalAddr(int sockfd)
{
	struct sockaddr_in6 localaddr;
	memset(&localaddr, 0, sizeof localaddr);
	socklen_t addrlen = (socklen_t)sizeof(localaddr);
	if (getsockname(sockfd, (struct sockaddr*)(&localaddr), &addrlen) < 0){
		NET_LOG_DEBUG("getsockname fail:fd:%d  %s\n", sockfd, strerror(errno));
	}
	return localaddr;
}

struct sockaddr_in6 netGetPeerAddr(int sockfd)
{
	struct sockaddr_in6 peeraddr;
	memset(&peeraddr, 0,sizeof peeraddr);
	socklen_t addrlen = (socklen_t)sizeof(peeraddr);
	if (getpeername(sockfd, (struct sockaddr*)(&peeraddr), &addrlen) < 0){
		NET_LOG_DEBUG("getpeername fail:fd:%d  %s\n", sockfd, strerror(errno));
	}
	return peeraddr;
}

int netGetPeerInfo(int sockfd,char* ip,int* port) {
	struct sockaddr_in6 peeraddr = netGetPeerAddr(sockfd);
	if (peeraddr.sin6_family == AF_INET6)
	{
		inet_ntop(peeraddr.sin6_family, &peeraddr.sin6_addr, ip, IP_MAX_LEN);
		*port = ntohs(peeraddr.sin6_port);
	}
	else
	{
		const struct sockaddr_in* raddr4 = (struct sockaddr_in*)(&peeraddr);
		inet_ntop(raddr4->sin_family, &raddr4->sin_addr, ip, IP_MAX_LEN);
		*port = ntohs(raddr4->sin_port);
	}
}

int netGetLocalInfo(int sockfd,char* ip,int* port) {
	struct sockaddr_in6 localaddr = netGetLocalAddr(sockfd);
	if (localaddr.sin6_family == AF_INET6)
	{
		inet_ntop(localaddr.sin6_family, &localaddr.sin6_addr, ip, IP_MAX_LEN);
		*port = ntohs(localaddr.sin6_port);
	}
	else
	{
		const struct sockaddr_in* raddr4 = (struct sockaddr_in*)(&localaddr);
		inet_ntop(raddr4->sin_family, &raddr4->sin_addr, ip, IP_MAX_LEN);
		*port = ntohs(raddr4->sin_port);
	}
}


int netIsSelfConnect(int sockfd) {
	struct sockaddr_in6 localaddr = netGetLocalAddr(sockfd);
	struct sockaddr_in6 peeraddr = netGetPeerAddr(sockfd);
	if (localaddr.sin6_family == AF_INET)
	{
		const struct sockaddr_in* laddr4 = (struct sockaddr_in*)(&localaddr);
		const struct sockaddr_in* raddr4 = (struct sockaddr_in*)(&peeraddr);
		return laddr4->sin_port == raddr4->sin_port
			&& laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
	}
	else if (localaddr.sin6_family == AF_INET6)
	{
		return localaddr.sin6_port == peeraddr.sin6_port
			&& memcmp(&localaddr.sin6_addr, &peeraddr.sin6_addr, sizeof localaddr.sin6_addr) == 0;
	}
	else
	{
		return 0;
	}
}