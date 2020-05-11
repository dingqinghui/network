#ifndef __timer_h__
#define __timer_h__


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <sys/time.h>
typedef void (*cb_func)(void*);


long long get_millisecond() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return  tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

///////////////////////////////////////////////////////////////////TIMER////////////////////////////////////////////////////////////////////////////////////////
typedef struct heap_timer {
	//�ӵ����õĶ�ʱ����ʱ����
	long long timeout;
	//��ʱ�ص�����
	cb_func callback;
	//�ص���������
	void* user_data;

} heap_timer;


heap_timer* create_timer(int timeout, cb_func cb, void* udata) {
	heap_timer* timer = (heap_timer*)malloc(sizeof(heap_timer));
	if (!timer)
		return NULL;

	long long seconds = get_millisecond();
	timer->timeout = seconds + timeout;
	timer->callback = cb;
	timer->user_data = udata;

	return timer;
}

void release_timer(heap_timer* timer) {
	free(timer);
}

///////////////////////////////////////////////////////////////////HEAP////////////////////////////////////////////////////////////////////////////////////////
typedef struct h_t_manager {
	//��ʱ��ָ������
	heap_timer** array;
	//��ǰ��ʱ������֧�ֵ����ʱ������
	int capacity;
	//��ǰ��ʱ�������ϵĶ�ʱ������
	int cur_size;
} h_t_manager;


static h_t_manager* tmanager = NULL;

int create_h_t_mgr(int cap){
	tmanager = malloc( sizeof(h_t_manager) );
	if (!tmanager) {
		exit(1);
		return -1;
	}
	tmanager->array = (heap_timer * *)malloc(cap * sizeof(h_t_manager));
	for (int i = 0; i < cap; ++i)
	{
		tmanager->array[i] = NULL;
	}
	tmanager->capacity = cap;
	tmanager->cur_size = 0;
	return 0;
}

int release_h_t_mgr() {
	if (!tmanager) {
		return -1;
	}
	for (int i = 0; i < tmanager->capacity; ++i)
	{
		heap_timer*  timer = tmanager->array[i];
		if (timer) {
			release_timer(timer);
		}
	}
	free(tmanager);
	return 0;
}


//�����������������1��
static int h_t_mgr_resize()
{
	heap_timer** temp = (heap_timer * *)
		malloc(2 * tmanager->capacity * sizeof(h_t_manager));

	if (!temp)
		return -1;

	for (int i = 0; i < 2 * tmanager->capacity; ++i)
		temp[i] = NULL;

	memcpy(temp, tmanager->array, sizeof(heap_timer*) * tmanager->capacity);

	tmanager->array = temp;
	tmanager->capacity = 2 * tmanager->capacity;
	return 0;
}

//��Ӷ�ʱ��
int h_t_mgr_add_timer(heap_timer* timer)
{
	if (!tmanager )
		return -1;

	if (tmanager->cur_size >= tmanager->capacity){
		h_t_mgr_resize();
	}

	int hole = tmanager->cur_size++;
	int parent = 0;
	for (; hole > 0; hole = parent)
	{
		parent = (hole - 1) / 2;
		if (tmanager->array[parent]->timeout <= timer->timeout)
		{
			break;
		}
		tmanager->array[hole] = tmanager->array[parent];
	}
	tmanager->array[hole] = timer;

	return hole;
}

int h_t_mgr_empty()
{
	return tmanager->cur_size == 0;
}

static void swap_mem(void* ap, void* bp, int size) {
	void* temp;
	temp = malloc(size);
	memcpy(temp, ap, size);
	memcpy(ap, bp, size);
	memcpy(bp, temp, size);
	free(temp);
}


static void percolate_down(int hole)
{
	heap_timer* temp = tmanager->array[hole];
	int flag = 0;
	int child = 0;
	for (; ((hole * 2 + 1) <= (tmanager->cur_size - 1)); hole = child)
	{
		flag = 0;
		child = hole * 2 + 1;

		//�����ҳ���ǰ�ڵ���С���ӽڵ�
		if ((child < (tmanager->cur_size - 1))
			&& (tmanager->array[child + 1]->timeout)
			< tmanager->array[child]->timeout)
		{
			++child;
		}
		//�Ƚϴ�ɾ���ڵ����С���ӽڵ㣬�����ھͽ���
		if (tmanager->array[child]->timeout < temp->timeout)
		{
			//����Ľ�����ʵ�����ڴ濽���ȽϺ�
			swap_mem(tmanager->array[hole], tmanager->array[child], sizeof(heap_timer));
		}
		else
		{
			break;
		}
		//�µ����˱Ƚ�
		temp = tmanager->array[child];
		//tmanager->array[hole] = temp;
	}
}

int h_t_mgr_pop_timer()
{
	if (h_t_mgr_empty(tmanager))
	{
		printf("!!!!!!!pop_timer->empty cur size\n");
		return -1;
	}
	if (tmanager->array[0])
	{
		release_timer(tmanager->array[0]);
		tmanager->array[0] = NULL;
		tmanager->array[0] = tmanager->array[--tmanager->cur_size];
		percolate_down( 0);
	}
	return 0;
}

int  h_t_mgr_min_timeout()
{
	if (h_t_mgr_empty(tmanager))
		return 0;

	return tmanager->array[0]->timeout - get_millisecond();
}



void h_t_mgr_tick()
{
	heap_timer* tmp = tmanager->array[0];
	long long cur = get_millisecond();
	while (!h_t_mgr_empty(tmanager))
	{
		if (!tmp)
		{
			break;
		}
		if (tmp->timeout > cur)
		{
			break;
		}
		if (tmanager->array[0]->callback)
			tmanager->array[0]->callback(tmanager->array[0]->user_data);

		h_t_mgr_pop_timer(tmanager);
		tmp = tmanager->array[0];
	}
}


#endif