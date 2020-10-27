
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "../include/timerheap.h"

static long long  getMillisecond(){
    long long t = 0;
    struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (long long)tv.tv_sec * 1000;
	t += tv.tv_usec / 1000;
    return t;
}


static timer_heap_t* T = 0;

//private
static void _th_percolate_down( int hole);//下虑
static void _th_percolate_down( int hole);//上浮
static void _th_resize();//扩容


int th_init(int capacity){
    T = malloc(sizeof(timer_heap_t));
    assert(T);
    T->array = (th_timer_t **)malloc(capacity * sizeof(th_timer_t));
    assert(T->array);
    T->size = 0;
    T->capacity = capacity;
    return 0;
}



void th_destroy() {
    assert(T);
    if(T->array){
        int i = 0;
        for (i = 0; i < T->size; ++i) {
            free(T->array[i]);
        }
        free(T->array);
    }
    free(T);
}

th_timer_t*  th_add_timer(long long timeout ,time_cb cb,void *user_data) {
    assert(T);
    th_timer_t* timer =  malloc(sizeof(th_timer_t));
    if (!timer)
        return -1;

    timer->cb_func =cb;
    timer->expire = timeout + getMillisecond();
    timer->user_data = user_data;
    timer->hole = T->size++;

    if (T->size >= T->capacity)
        _th_resize(T);

    T->array[timer->hole] = timer;

    _th_percolate_up(T->size-1);

    return timer;
}

void _th_percolate_up( int hole ){
    int parent = 0;
    for (; hole > 0; hole = parent) {
        parent = (hole) >> 1;

        if (T->array[parent]->expire <= T->array[hole]->expire)
            break;

        void* tmp = T->array[hole];

        T->array[hole] = T->array[parent];
        (T->array[hole])->hole = hole;

        T->array[parent] = tmp;
        (T->array[parent])->hole = parent;

        hole = parent;
    }
}


void th_del_timer(th_timer_t* timer) {
    assert(T);
    assert(timer);

    T->array[timer->hole] = T->array[--T->size];
    (T->array[timer->hole])->hole = timer->hole;
    _th_percolate_down( timer->hole);
}

th_timer_t* th_top_timer(){
    assert(T);
    if (th_empty(T))
        return 0;

    return T->array[0];
}

void th_pop_timer(){
    assert(T);
    if (th_empty(T))
        return ;

    if (T->array[0]) {
        free(T->array[0]);
        T->array[0] = T->array[--T->size];
        (T->array[0])->hole = 0;
        _th_percolate_down(0);
    }
}

void th_tick(){
    assert(T);
    th_timer_t *tmp = T->array[0];
    long long curtime = getMillisecond();

    //循环处理堆中到期的定时器
    while (!th_empty(T)) {
        if (!tmp)
            break;

        if (tmp->expire > curtime)
            break;//堆顶定时器未到期

        if (T->array[0]->cb_func)
           T->array[0]->cb_func(T->array[0]->user_data);

        th_pop_timer();
        tmp = T->array[0];
    }
}

int th_empty(){
    assert(T);
    return (T->size == 0) ? 1 : 0;
}


void _th_percolate_down( int hole){
    th_timer_t *tmp = T->array[hole];
    int child = 0;

    for (; ((hole * 2 + 1) <= (T->size - 1)); hole = child)
    {
        child = hole * 2 + 1;
        if ((child < (T->size - 1)) && 
            (T->array[child]->expire >= T->array[child + 1]->expire)){
            ++child;
        }

        if (T->array[child]->expire >= tmp->expire)
            break;

        T->array[hole] = T->array[child];
        (T->array[hole])->hole = hole;
    }
    
    T->array[hole] = tmp;
    (T->array[hole])->hole = hole;
}

void _th_resize() {
    T->array = (th_timer_t **)realloc(T->array, T->capacity * 2 * sizeof(th_timer_t));
    assert(T->array);
    T->capacity *= 2;
}


void th_print(){
    for(int i = 0;i < T->size;++i){
        printf("index:%d expire:%d  hole:%d\n",i,(T->array[i])->expire,(T->array[i])->hole);
    }
} 