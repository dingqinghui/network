


#include <stdio.h>
#include <stdarg.h>
#include <time.h>
 #include <sys/time.h>

#include "zlog.h"


static long long  getMillisecond(){
    long long  t = 0;
    struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (long long)tv.tv_sec * 1000;
	t += tv.tv_usec / 1000;
    return t;
}


int main(int argc, char** argv)

{

	int rc;
	zlog_category_t *c;

	rc = zlog_init("test_hello.conf");

	if (rc) {

	printf("init failed\n");
	return -1;

	}

	c = zlog_get_category("my_cat");

	if (!c) {

	printf("get cat fail\n");

	zlog_fini();
	return -2;

	}

	long long old = getMillisecond();
	for (size_t i = 0; i < 100000; i++)
	{
		zlog_info(c, "hello, zlog");
	}
	
	zlog_info(c,"LOG_DEBUG", getMillisecond()-old);
	

	zlog_fini();

	return 0;

} 


// int main()
// {
// 	long long old = getMillisecond();
// 	for (size_t i = 0; i < 10000; i++)
// 	{
// 		LOGC("LOG_DEBUG", "a=%d", 10);
// 	}
	
// 	LOGC("LOG_DEBUG", "a=%d", getMillisecond()-old);

// 	return 0;
// }