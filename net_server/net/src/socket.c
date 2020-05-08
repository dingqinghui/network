
#include <sys/types.h>         
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "../include/socket.h"


char* socket_print(st_socket* so) {
	char msg[50] = " id:%d  fd:%d  stat:%d ";
	sprintf(msg, so->id, so->fd, so->stat);
	return msg;
}


int  socket_set_noblock(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0) {
		return flag;
	}

	return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}


st_socket* socket_create()
{
	struct st_socket* so = malloc(sizeof(struct st_socket));
	if (!so) {
		printf("malloc socket fail memory out.\n");
		return NULL;
	}

	so->id = 0;
	so->fd = 0;
	so->stat = SOCKET_STAT_CLOSE;

	return so;
}

void socket_release(st_socket* so)
{
	if (so) {
		socket_close(so);
		free(so);
	}
}

bool socket_is_close(st_socket* so) {
	return so && so->stat == SOCKET_STAT_CLOSE;
}


int socket_close(st_socket* so) {
	if (so && so->fd > 0 && !socket_is_close(so)) {
		close(so->fd);
	}
}


bool socket_is_lisent(st_socket* so) {
	return so && so->stat == SOCKET_STAT_LISENT;
}
bool socket_is_esta(st_socket* so) {
	return so && so->stat == SOCKET_STAT_ESTABLISHED;
}
bool socket_is_connecting(st_socket* so) {
	return so && so->stat == SOCKET_STAT_CONNECTING;
}

void socket_set_stat(st_socket* so, int stat) {
	if (so)
		so->stat = stat;
}

void socket_get_stat(st_socket* so) {
	return so->stat;
}

void socket_set_fd(st_socket* so, int fd) {
	if (so)
		so->fd = fd;
}

void socket_set_id(st_socket* so, int id) {
	if (so)
		so->id = id;
}


st_socket* socket_lisent(char* IP, int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		return NULL;
	}

	if (socket_set_noblock(sockfd) < 0)
		return NULL;


	int reuse = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		close(sockfd);
		return NULL;
	}


	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(port);


	if (bind(sockfd, (struct sockaddr*) & addr, sizeof(addr)) < 0) {
		close(sockfd);
		return NULL;
	}


	if (listen(sockfd, 10) < 0) {
		close(sockfd);
		return NULL;
	}

	st_socket* so = socket_create();
	if (!so) {
		close(sockfd);
		printf("lisent fail socket_create not memory\n");
		return NULL;
	}

	socket_set_stat(so, SOCKET_STAT_LISENT);
	socket_set_fd(so, sockfd);
	return so;
}

st_socket* socket_connect(char* IP, int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		return NULL;
	}

	socket_set_noblock(sockfd);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(port);


	int ret = connect(sockfd, (struct sockaddr_in*) & addr, sizeof(addr));
	if (ret < 0) {
		if (errno == EINPROGRESS) {

			st_socket* so = socket_create();
			if (!so) {
				close(sockfd);
				printf("lisent fail socket_create not memory\n");
				return NULL;
			}
			socket_set_stat(so, SOCKET_STAT_CONNECTING);
			socket_set_fd(so, sockfd);
			return so;
		}
		else {
			close(sockfd);
			return NULL;
		}

	}

	//connect success
	st_socket* so = socket_create(sockfd);
	if (!so) {
		close(sockfd);
		return NULL;
	}
	socket_set_stat(so, SOCKET_STAT_ESTABLISHED);
	socket_set_fd(so, sockfd);

	return so;
}

st_socket* socket_accept(st_socket* l_socket)
{
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	int c_socket = accept(l_socket->fd, (struct sockaddr*) & remoteAddr, &nAddrlen);
	if (c_socket < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
		}
		else {
		}
		return NULL;
	}

	//connect success
	st_socket* so = socket_create(c_socket);
	if (!so) {
		close(c_socket);
		return NULL;
	}
	socket_set_stat(so, SOCKET_STAT_ESTABLISHED);
	socket_set_fd(so, c_socket);

	return so;
}

int socket_read(st_socket* so, char* buffer, int size) {
	int nread = read(so->fd, buffer, size);
	if (nread < 0)
		return -1;
	return nread;
}


int socket_write(st_socket* so, char* buffer, int size) {
	int nwrite = write(so->fd, buffer, size);

	if (nwrite < 0)
		return -1;
	return nwrite;
}

