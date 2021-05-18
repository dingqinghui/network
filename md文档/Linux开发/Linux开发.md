## 工作目录
每个进程都有自己的工作目录，此目录是所有相对路径名搜索的起点。默认为进程启动时的工作目录。

```c
//获取工作目录
char buf[80];   
getcwd(buf,sizeof(buf));   
printf("current working directory: %s\n", buf); 
//更改工作目录
int chdir(const char *path);
```

## 多线程私有数据
多线程私有数据pthread_key_create，一个key，多个值。
如常见的errno，它返回标准的错误码。errno不应该是一个局部变量。几乎每个函数都应该可以访问他，但他又不能作为是一个全局变量。否则在一个线程里输出的很可能是另一个线程的
线程私有数据采用了一键多值的技术，即一个键对应多个值。访问数据时都是通过键值来访问，好像是对一个变量进行访问，其实是在访问不同的数据。
原理：进程空间使用数据存放每个key的使用情况和销毁函数，create 在这个数据中找到一个空闲的key。get/set 通过key在线程私有数据数组中找到数据地址。
![img](https://i.loli.net/2021/05/14/bEOa8hlmpitR4cy.jpg)

```c
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int pthread_setspecific(pthread_key_t key,const void *pointer));
void *pthread_getspecific(pthread_key_t key);
int pthread_key_delete(pthread_key_t key);
```



## 文件锁

```c
#include<sys/file.h>
int flock(int fd,int operation);
```

## 重定向标准输入 输出 错误
```c
int nfd = open("/dev/null", O_RDWR);
dup2(nfd, 0);  
dup2(nfd, 1);
dup2(nfd, 2);
```

## unlink  安全删除进程临时文件
删除目录项并减少一个连接数，如果链接数为0并且没有任何进程打开该文件，该文件内容才能被真正删除，但是若又进程打开了该文件，则文件暂时不删除直到所有打开该文件的进程都结束时文件才能被删除。

调用后目录项立刻被删除，而文件内从需要满足连接数为0才删除。也就是说你再目录中看不到文件，但是却可以对文件的内容进行操作。

## daemon 创建守护进程
```c
/*
当 nochdir为零时，当前目录变为根目录，否则不变；
当 noclose为零时，标准输入、标准输出和错误输出重导向为/dev/null，也就是不输出任何信 息，否则照样输出。
*/
int daemon(int nochdir, int noclose);
```

## 检测进程是否存在
```c
// kill发送信号给进程.如果 sig = 0，不发送信号，但是会进行错误检查。通过errno == ESRCH检测进程是否存在
if (kill(pid, 0) && errno == ESRCH)
```

## SIGUP信号

1、终端关闭时，该信号被发送到session首进程以及作为job提交的进程（即用 & 符号提交的进程，后台进程）；
2、session首进程退出时，该信号被发送到该session中的前台进程组中的每一个进程；daemon会触发这个过程注意处理
3、若父进程退出导致进程组成为孤儿进程组，且该进程组中有进程处于停止状态（收到SIGSTOP或SIGTSTP信号），该信号会被发送到该进程组中的每一个进程。

这个信号的默认操作为终止进程

## daemon创建过程

![](https://i.loli.net/2021/05/14/xGXqZB5deCwInzT.png)

##  多线程原子操作

Gcc 4.1.2版本之后，对X86或X86_64支持内置原子操作。即可对1、2、4、8字节的数值或指针类型，进行原子加/减/与/或/异或等操作。性能优于线程锁。
```c
type __sync_fetch_and_add (type *ptr, type value, ...)
// 将value加到*ptr上，结果更新到*ptr，并返回操作之前*ptr的值
type __sync_fetch_and_sub (type *ptr, type value, ...)
// 从*ptr减去value，结果更新到*ptr，并返回操作之前*ptr的值
type __sync_fetch_and_or (type *ptr, type value, ...)
// 将*ptr与value相或，结果更新到*ptr， 并返回操作之前*ptr的值
type __sync_fetch_and_and (type *ptr, type value, ...)
// 将*ptr与value相与，结果更新到*ptr，并返回操作之前*ptr的值
type __sync_fetch_and_xor (type *ptr, type value, ...)
// 将*ptr与value异或，结果更新到*ptr，并返回操作之前*ptr的值
type __sync_fetch_and_nand (type *ptr, type value, ...)
// 将*ptr取反后，与value相与，结果更新到*ptr，并返回操作之前*ptr的值
type __sync_add_and_fetch (type *ptr, type value, ...)
// 将value加到*ptr上，结果更新到*ptr，并返回操作之后新*ptr的值
type __sync_sub_and_fetch (type *ptr, type value, ...)
// 从*ptr减去value，结果更新到*ptr，并返回操作之后新*ptr的值
type __sync_or_and_fetch (type *ptr, type value, ...)
// 将*ptr与value相或， 结果更新到*ptr，并返回操作之后新*ptr的值
type __sync_and_and_fetch (type *ptr, type value, ...)
// 将*ptr与value相与，结果更新到*ptr，并返回操作之后新*ptr的值
type __sync_xor_and_fetch (type *ptr, type value, ...)
// 将*ptr与value异或，结果更新到*ptr，并返回操作之后新*ptr的值
type __sync_nand_and_fetch (type *ptr, type value, ...)
// 将*ptr取反后，与value相与，结果更新到*ptr，并返回操作之后新*ptr的值
bool __sync_bool_compare_and_swap (type *ptr, type oldval type newval, ...)
// 比较*ptr与oldval的值，如果两者相等，则将newval更新到*ptr并返回true
type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
// 比较*ptr与oldval的值，如果两者相等，则将newval更新到*ptr并返回操作之前*ptr的值
__sync_synchronize (...)
// 发出完整内存栅栏
type __sync_lock_test_and_set (type *ptr, type value, ...)
// 将value写入*ptr，对*ptr加锁，并返回操作之前*ptr的值。即，try spinlock语义
void __sync_lock_release (type *ptr, ...)
// 将0写入到*ptr，并对*ptr解锁。即，unlock spinlock语义

```

```c
// 实现多线程自旋锁
struct spinlock {
	int lock;
};

static inline void
spinlock_init(struct spinlock *lock) {
	lock->lock = 0;
}

static inline void
spinlock_lock(struct spinlock *lock) {
	while (__sync_lock_test_and_set(&lock->lock,1)) {}
}

static inline int
spinlock_trylock(struct spinlock *lock) {
	return __sync_lock_test_and_set(&lock->lock,1) == 0;
}

static inline void
spinlock_unlock(struct spinlock *lock) {
	__sync_lock_release(&lock->lock);
}

static inline void
spinlock_destroy(struct spinlock *lock) {
	(void) lock;
}
```
```c
//数值+- 原子操作
#define ATOM_CAS(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)//CAS compare and swap
#define ATOM_CAS_POINTER(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_INC(ptr) __sync_add_and_fetch(ptr, 1)
#define ATOM_FINC(ptr) __sync_fetch_and_add(ptr, 1)
#define ATOM_DEC(ptr) __sync_sub_and_fetch(ptr, 1)
#define ATOM_FDEC(ptr) __sync_fetch_and_sub(ptr, 1)
#define ATOM_ADD(ptr,n) __sync_add_and_fetch(ptr, n)
#define ATOM_SUB(ptr,n) __sync_sub_and_fetch(ptr, n)
#define ATOM_AND(ptr,n) __sync_and_and_fetch(ptr, n)
```

## dlopen 动态加载动态库
```c
#include <dlfcn.h>
void *dlopen(const char *filename, int flag);
char *dlerror(void);
void *dlsym(void *handle, const char *symbol);
int dlclose(void *handle);
//Link with -ldl.
```

## pthead_once
多线程中，pthead_once的参数init_routine只会被执行一次。可用于多线程的初始化工作。
```C
int pthread_once(pthread_once_t *once_control,
void (*init_routine)(void));
```

## setjmp longjmp
goto 只能在函数内跳转short jump。longjmp 可以跨函数跳转。
setjmp保存函数的堆栈到env，第一次调用返回 0，通过longjump跳转到这里则返回longjmp的参数val。
longjmp 跳转到env继续执行。
```C
int 	setjmp(jmp_buf env);
void 	longjmp(jmp_buf env, int val);
```
