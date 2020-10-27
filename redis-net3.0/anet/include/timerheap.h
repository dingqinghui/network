

#ifndef __TIME_HEAP_H__
#define __TIME_HEAP_H__


#include <time.h>

typedef void (*time_cb)(void *);

typedef struct _th_timer {
    int id;
    long long expire;          //定时器生效的绝对时间
    time_cb cb_func;
    void *user_data;
    int hole;
} th_timer_t;

typedef struct _time_heap {
    th_timer_t **array;     //堆数组
    int capacity;           //堆数组容量
    int size;               //堆数组当前包含元素的个数
} timer_heap_t;

int th_init( int capacity);

void th_destroy();

th_timer_t*  th_add_timer(long long timeout ,time_cb cb,void *user_data);
void th_del_timer(th_timer_t* timer);

void th_tick();
int th_empty();

void th_pop_timer();

#endif