#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "../include/net_server.h"

net_server* net_server_create(int maxfd,void* connect_callback,void* accept_callback, void* recv_callback, void* close_callback, void* error_callback, void* timeout_callback)
{
	s = malloc(sizeof(net_server));
	if (!s) {
		exit(1);
		return;
	}

	s->epfd = sp_create();
	if (!sp_invaild(s->epfd)) {
		exit(1);
		return;
	}

	s->events = malloc(sizeof(struct event) * maxfd);
	if (s->events == NULL) {
		exit(1);
		return;
	}

	s->socket_slot = malloc(sizeof(void*) * maxfd);
	if (s->socket_slot == NULL) {
		exit(1);
		return;
	}

	for (int i = 0; i < maxfd; ++i) {
		s->socket_slot[i] = NULL;
	}

	if (pipe(s->pipes) < 0) {
		exit(1);
		return;
	}
		


	s->maxfd = maxfd;
	s->event_index = 0;
	s->event_n = 0;

	s->connect_callback = connect_callback;
	s->accept_callback = accept_callback;
	s->recv_callback = recv_callback;
	s->close_callback = close_callback;
	s->error_callback = error_callback;
	s->timeout_callback = timeout_callback;

	sp_add(s->epfd,s->pipes[0],NULL);
	return s;
}


void net_server_release()
{
	if (!s)
		return;

	close(s->pipes[0]);
	close(s->pipes[1]);

	sp_release(s->epfd);

	for (int i = 0; i < s->maxfd; ++i) {
		st_socket* so = s->socket_slot[i];
		if (!so)
			continue;

		socket_release(so);
		s->socket_slot[i] = NULL;
	}

	free(s);
}

//todo
static int on_reverse_id() {
	for (int i = 0; i < s->maxfd; ++i) {
		st_socket* so = s->socket_slot[i];
		if (!so) {
			return i;
		}
	}
	printf("server reverse socket id fail\n");
	return -1;
}

static int on_add_socket(st_socket* so) {
	if (!so)
		return -1;

	int id = on_reverse_id();
	if (id < 0)
		return -1;

	socket_set_id(so, id);

	s->socket_slot[id] = so;

	return 0;
}


static int on_del_socket(id) {
	s->socket_slot[id] = NULL;
}

int net_server_lisent(char* IP, int port) {
	st_socket* so = socket_lisent(IP, port);
	if (!so) {
		printf("lisent fail errno:%d msg:%s\n", errno, strerror(errno));
		return -1;
	}
	//save socket to server
	if (on_add_socket(so) < 0) {
		socket_release(so);
		return -1;
	}

	//add to epoll
	if (sp_add(s->epfd, so->fd, so) < 0) {
		on_del_socket(so->id);
		socket_release(so);
		return -1;
	}

	printf("lisent success  ip:%s port:%d   id:%d \n", IP, port, so->id);
	return so->id;
}


int net_server_connect(char* IP, int port) {
	st_socket* so = socket_connect(IP, port);
	if (so) {
		if (on_add_socket(so) < 0) {
			socket_release(so);
			return -1;
		}

		//add to epoll
		if (sp_add(s->epfd, so->fd, so) < 0) {
			on_del_socket(so->id);
			socket_release(so);
			return -1;
		}

		if (socket_is_connecting(so)) {

			if (sp_write(s->epfd, so->fd, so, true) < 0) {
				on_del_socket(so->id);
				socket_release(so);

				return -1;
			}
			printf("connection cannot be completed immediately  ip:%s port:%d fd:%d\n", IP, port, so->fd);
			return so->id;
		}
		else {
			if (s->connect_callback)
				((connect_fun)(s->connect_callback))(so->id);

			printf("connect success ip:%s port:%d\n", IP, port);
			return so->id;
		}
	}
	else {
		printf("connect fail ip:%s port:%d  errno:%d  msg:%s\n", IP, port, errno, strerror(errno));
		return -1;
	}
	return -1;
}


int  net_server_send(int id, char* buffer, int size) {
	st_socket* so = s->socket_slot[id];
	if (!so)
		return 0;

	int ret = socket_write(so, buffer, size);
	if (ret <= 0) {
		if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
		}
		else {
			if (s->error_callback)
				((error_fun)(s->error_callback))(so->id);

			sp_del(s->epfd, so->fd);
			on_del_socket(so->id);
			socket_release(so);
			return ret;
		}
	}
	return ret;
}



static  int on_report_error(st_socket* so) {

	if (s->error_callback)
		((error_fun)(s->error_callback))(so->id);
	printf("epoll wait error event  id:%d\n", so->id);
	return 0;
}

static int on_report_connect(st_socket* so) {
	int error;
	int len = sizeof(error);
	int code = getsockopt(so->fd, SOL_SOCKET, SO_ERROR, &error, &len);

	if (code < 0 || error) {
		char* strerr = NULL;
		int err = 0;
		if (code >= 0) {
			strerr = strerror(error);
			err = error;
		}
		else {
			strerr = strerror(errno);
			err = errno;
		}

		sp_del(s->epfd, so->fd);
		on_del_socket(so->id);
		socket_release(so);

		if (s->close_callback)
			((close_fun)(s->close_callback))(so->id);

		printf("connect fail errno:%d msg:%s\n", err, strerr);
		return 0;
	}
	else {
		socket_set_stat(so, SOCKET_STAT_ESTABLISHED);

		sp_write(s->epfd, so->fd, so, false);

		if (s->connect_callback)
			((connect_fun)(s->connect_callback))(so->id);

		printf("connect success \n");
		return 0;
	}
}

static int on_report_accpet(st_socket* l_so) {

	st_socket* n_so = socket_accept(l_so);
	if (l_so) {

		if (on_add_socket(n_so) < 0) {
			socket_release(n_so);
			return -1;
		}

		//add to epoll
		if (sp_add(s->epfd, n_so->fd, n_so) < 0) {
			on_del_socket(n_so->id);
			socket_release(n_so);
			return -1;
		}

		if (s->accept_callback)
			((accpet_fun)(s->accept_callback))(n_so->id);

		return 0;
	}
	return -1;
}


static int on_report_read(st_socket* so) {
	char* buffer = malloc(sizeof(char) * 65535);

	int nread = socket_read(so, buffer, 65535);
	if (nread <= 0) {
		int err = errno;
		if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
			return -1;
		}
		if (s->close_callback)
			((close_fun)(s->close_callback))(so->id);

		printf("read size = 0 close fd:%d id:%d errno:%d  \n", so->fd, so->id, errno);

		sp_del(s->epfd, so->fd);
		on_del_socket(so->id);
		socket_release(so);
	}
	else {
		if (s->recv_callback)
			((recv_fun)(s->recv_callback))(so->id,buffer,nread);

		printf("on_report_read buffer:%s\n", buffer);
	}
	return 0;
}

static int on_report_timeout() {
	((timeout_fun)s->timeout_callback)();
}

int net_server_epoll(int timeout){
	timeout = timeout == 0 ? -1 : timeout;
	int cnt = sp_wait(s->epfd, s->events, s->maxfd, timeout);
	s->event_index = 0;
	s->event_n = cnt;
	if (s->event_n < 0){
		return s->event_n;
	}
	//timeout
	else if (s->event_n == 0){
		on_report_timeout();
	}
	else {
		for (; s->event_index < s->event_n; ) {
			struct event  e = s->events[s->event_index];
			s->event_index++;

			st_socket* so = (st_socket*)e.udata;
			if (!so) {
				printf("deal epoll event fail st_socket not exist\n");
				continue;
			}

			if (e.write) {
				if (socket_is_connecting(so)) {
					on_report_connect(so);
				}
			}
			if (e.read) {
				if (socket_is_lisent(so)) {
					on_report_accpet(so);
				}
				else if (socket_is_esta(so)) {
					on_report_read(so);
				}
			}
			if (e.error)
			{
				on_report_error(so);
			}
		}
	}
}




