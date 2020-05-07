#include <sys/types.h>         
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include "../include/socket_event.h"

int register_event(st_callback* sc, int event_type, void* con_call) {
	if (!sc)
		return -1;
	struct st_socket* so = sc->so;
	int id = so->id;
	void* callback = sc->callback_list[event_type];
	if (callback) {
		printf("event_mgr_register fail callback exist id:%d  event_type:%d\n", id, event_type);
		return -1;
	}

	sc->callback_list[event_type] = con_call;
	printf("event_mgr_register success id:%d  event_type:%d\n", id, event_type);
	return 0;
}

int emit_event(st_callback* sc, int event_type, event_param* param) {
	if (!sc)
		return -1;
	if (!sc->so)
		return -1;

	if (socket_is_close(sc->so))
		return -1;

	void* callback = sc->callback_list[event_type];
	if (!callback) {
		return -1;
	}

	int id = sc->so->id;


	switch (event_type)
	{
	case SOCKET_EVENT_CONNECTED:
		((connect_fun)callback)(id);
		break;
	case SOCKET_EVENT_ACCPET:
	{
		int n_id = param->ud;
		((accpet_fun)callback)(n_id);
	}
	break;
	case SOCKET_EVENT_RECV:
		((recv_fun)callback)(id, param->ud, param->size);
		break;
	default:
		break;
	}

	printf("event_mgr_emit success id:%d  type:%d\n", id, event_type);
	return 0;
}


