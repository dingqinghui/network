// ConsoleApplication2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include "buffer.h"

int main()
{

	st_buffer* buffer = create_buffer();
	for (int index = 1; index < 100; index++)
	{
		int nwrite = write_to_buffer(buffer, "123456", 6);
		printf("write_to_buffer size :%d \n", nwrite);
	}
	print_buffer(buffer);
	for (int index = 1; index < 50; index++) {
		char buf[15];
		buf[14] = '\0';
		int nread = read_from_buffer(buffer, buf, 14);
		printf("read_from_buffer size :%d \n", nread);
		printf("buffer is :%s \n", buf);
	}
  
}

