

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>

#include "../include/server.h"

server* server_create(int maxfd)
{
	s = malloc(sizeof(server));
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


	s->maxfd = maxfd;
	s->event_index = 0;
	s->event_n = 0;

	return s;
}


void server_release()
{
	if (!s)
		return;

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
int server_reverse_id() {
	for (int i = 0; i < s->maxfd; ++i) {
		st_socket* so = s->socket_slot[i];
		if (!so) {
			return i;
		}
	}
	printf("server reverse socket id fail\n");
	return -1;
}

int server_add_socket(st_socket* so) {
	if (!so)
		return -1;

	int id = server_reverse_id();
	if (id < 0)
		return -1;

	socket_set_id(so, id);

	s->socket_slot[id] = so;

	return 0;
}
int server_del_socket(id) {
	s->socket_slot[id] = NULL;
}

int server_lisent(char* IP, int port, void* callback) {
	st_socket* so = socket_lisent(IP, port);
	if (!so) {
		printf("lisent fail errno:%d msg:%s\n", errno, strerror(errno));
		return -1;
	}
	//save socket to server
	if (server_add_socket(so) < 0) {
		socket_release(so);
		return -1;
	}

	//add to epoll
	if (sp_add(s->epfd, so->fd, so) < 0) {
		server_del_socket(so->id);
		socket_release(so);
		return -1;
	}

	//register socket event callback

	socket_register_event(so, SOCKET_EVENT_ACCPET, callback);

	printf("lisent success  ip:%s port:%d   id:%d \n", IP, port, so->id);
	return so->id;
}


int server_connect(char* IP, int port, void* callback) {
	st_socket* so = socket_connect(IP, port);
	if (so) {
		if (server_add_socket(so) < 0) {
			socket_release(so);
			return -1;
		}

		//add to epoll
		if (sp_add(s->epfd, so->fd, so) < 0) {
			server_del_socket(so->id);
			socket_release(so);
			return -1;
		}
		//register event
		socket_register_event(so, SOCKET_EVENT_CONNECTED, callback);

		if (socket_is_connecting(so)) {

			if (sp_write(s->epfd, so->fd, so, true) < 0) {
				server_del_socket(so->id);
				socket_release(so);

				return -1;
			}
			printf("connection cannot be completed immediately  ip:%s port:%d fd:%d\n", IP, port, so->fd);
			return so->id;
		}
		else {
			socket_emit_event(so, SOCKET_EVENT_CONNECTED, NULL);
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


int  server_send(int id, char* buffer, int size) {
	st_socket* so = s->socket_slot[id];
	if (!so)
		return 0;

	int ret = socket_write(so, buffer, size);
	if (ret <= 0) {
		if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
		}
		else {
			sp_del(s->epfd, so->fd);
			server_del_socket(so->id);
			socket_release(so);
			return ret;
		}
	}
	return ret;
}

void on_network()
{
	int ret = on_server_epoll(&event);
}

int on_server_epoll()
{

	if (s->event_index >= s->event_n) {
		int cnt = sp_wait(s->epfd, s->events, s->maxfd);
		s->event_index = 0;
		s->event_n = cnt;
	}
	else {

		for (; ; ) {
			struct event  e = s->events[s->event_index];
			s->event_index++;

			st_socket* so = (st_socket*)e.udata;
			if (!so) {
				printf("deal epoll event fail st_socket not exist");
				continue;
			}

			if (e.write) {
				if (socket_is_connecting(so)) {
					return on_report_connect(so);
				}
			}
			if (e.read) {
				if (socket_is_lisent(so)) {
					return on_report_accpet(so);
				}
				else if (socket_is_esta(so)) {
					return on_report_read(so);
				}
			}
			if (e.error)
			{
				return on_report_read(so);
			}
		}
	}
}

int on_report_error(st_socket* so) {

	socket_emit_event(so, SOCKET_EVENT_ERROR, NULL);

	printf("epoll wait error event  id:%d\n", so->id);
	return 0;
}

int on_report_connect(st_socket* so) {
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
		server_del_socket(so->id);
		socket_release(so);

		socket_emit_event(so, SOCKET_EVENT_CLOSE, NULL);

		printf("connect fail errno:%d msg:%s\n", err, strerr);
		return 0;
	}
	else {
		socket_set_stat(so, SOCKET_STAT_ESTABLISHED);

		sp_write(s->epfd, so->fd, so, false);


		socket_emit_event(so, SOCKET_EVENT_CONNECTED, NULL);

		printf("connect success %s\n", socket_print(so));
		return 0;
	}
}

int on_report_accpet(st_socket* l_so) {

	st_socket* n_so = socket_accept(l_so);
	if (l_so) {

		if (server_add_socket(n_so) < 0) {
			socket_release(n_so);
			return -1;
		}

		//add to epoll
		if (sp_add(s->epfd, n_so->fd, n_so) < 0) {
			server_del_socket(n_so->id);
			socket_release(n_so);
			return -1;
		}

		event_param param;
		param.ud = n_so->id;
		socket_emit_event(l_so, SOCKET_EVENT_ACCPET, &param);

		return 0;
	}
	return -1;
}


int on_report_read(st_socket* so) {
	char* buffer = malloc(sizeof(char) * 65535);

	int nread = socket_read(so, buffer, 65535);
	if (nread <= 0) {
		int err = errno;
		if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
			return -1;
		}

		sp_del(s->epfd, so->fd);
		server_del_socket(so->id);
		socket_release(so);

		socket_emit_event(so, SOCKET_EVENT_CLOSE, NULL);

		//strerror(errno)
		printf("read size = 0 close fd:%d id:%d errno:%d  \n", so->fd, so->id, errno);
	}
	else {
		event_param param;
		param.ud = buffer;
		param.size = nread;
		socket_emit_event(so, SOCKET_EVENT_RECV, &param);

		printf("on_report_read buffer:%s\n", buffer);
	}
	return 0;
}


int server_register_event(int id, int event_type, void* callback) {
	st_socket* so = s->socket_slot[id];
	if (!so)
		return -1;
	return socket_register_event(so, event_type, callback);
}



void on_work()
{

}


void server_run()
{
	while (true)
	{
		//socket处理
		on_network();
		//逻辑处理

		sleep(1);
	}
}


