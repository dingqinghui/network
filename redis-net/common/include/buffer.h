#ifndef __BUFFER_H__
#define __BUFFER_H__

typedef struct  st_block
{
	char* data;
	int   size;
	int   begin;
	int   end;
	struct st_block* next;

}st_block;

typedef struct
{
	st_block* head;
	st_block* tail;
	st_block* free;
}st_buffer;


st_buffer* create_buffer();
int release_buffer(st_buffer* buffer);
int write_to_buffer(st_buffer* buffer, const char* pdata, int nsize) ;
int read_from_buffer(st_buffer* buffer, char* pdata, int nsize) ;
int print_buffer(st_buffer* buffer);


#endif