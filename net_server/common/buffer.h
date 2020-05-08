#ifndef __buffer_h__
#define __buffer_h__

#include <stdlib.h>
#include <string.h>
typedef struct  st_block
{
	char* data;
	int   size;
	int   begin;
	int   end;
	struct st_block* next;

}st_block;

st_block* create_block(int size) {
	st_block* block = (st_block*)malloc(sizeof(st_block));
	if (!block) {
		return NULL;
	}
	char* data = (char*)malloc(sizeof(char) * size);
	if (!data) {
		return NULL;
	}
	block->data = data;
	block->size = size;
	block->begin = 0;
	block->end = 0;
	block->next = NULL;

	return block;
}

int release_block(st_block* block) {
	if (block->data)
		free(block->data);
	free(block);
	return 0;
}

int write_to_block(st_block* block, const char* buffer, int nsize) {
	if (!block)
		return 0;

	int nfree = block->size - block->end;
	if (nfree <= 0)
		return 0;

	int nwrite = nfree >= nsize ? nsize : nfree;
	memcpy(block->data + block->end, buffer, nwrite);
	block->end += nwrite;
	return nwrite;
}
int is_empty_block(st_block* block) {
	if (!block)
		return 0;

	return block->begin == block->end;
}

int reset_block(st_block* block) {
	if (!block)
		return -1;
	block->begin = 0;
	block->end = 0;
}
int read_from_block(st_block* block, char* buffer, int nsize) {
	if (!block)
		return 0;
	int nuse = block->end - block->begin;
	if (nuse <= 0)
		return 0;

	int nread = nuse >= nsize ? nsize : nuse;
	memcpy(buffer, block->data + block->begin, nread);
	block->begin += nread;

	if (is_empty_block(block))
	{
		reset_block(block);
	}
	return nread;
}



typedef struct
{
	st_block* head;
	st_block* tail;
	st_block* free;
	int nsize;
}st_buffer;

st_buffer* create_buffer() {

	st_buffer* buffer = (st_buffer*)malloc(sizeof(st_buffer));
	if (!buffer)
		return NULL;

	buffer->head = NULL;
	buffer->tail = NULL;
	buffer->free = NULL;

	return buffer;
}

int release_buffer(st_buffer* buffer) {
	if (!buffer)
		return -1;

	st_block* block = buffer->head;
	while (block) {
		st_block* o_block = block;
		block = block->next;
		release_block(o_block);
	}
	free(buffer);
	return 0;
}


int write_to_buffer(st_buffer* buffer, const char* pdata, int nsize) {
	if (!buffer)
		return 0;

	int nwrite = 0;
	if (buffer->free)
	{
		nwrite = write_to_block(buffer->free, pdata, nsize);
		buffer->nsize += nwrite;
		if (nwrite < nsize) {
			//free block out of memory
			pdata = pdata + nwrite;
			nsize = nsize - nwrite;
		}
		else
		{
			return nwrite;
		}
	}

	st_block* block = create_block(nsize * 2);
	if (!block)
		return nwrite;

	nwrite += write_to_block(block, pdata, nsize);
	buffer->nsize += nwrite;
	if (buffer->tail) {
		buffer->tail->next = block;
		buffer->tail = block;
	}
	else
		buffer->tail = block;

	if (buffer->head == NULL)
		buffer->head = block;

	buffer->free = block;

	return nwrite;
}

int read_from_buffer(st_buffer* buffer, char* pdata, int nsize) {
	if (nsize <= 0)
		return 0;
	if (!buffer)
		return 0;

	if (buffer->head == NULL)
		return 0;

	int nread = 0;
	if (buffer->head == buffer->tail) {
		nread = read_from_block(buffer->head, pdata, nsize);
		buffer->nsize -= nread;
		return nread;
	}

	//traverse buffer list  read data.  buffer->free->next must not have data
	
	st_block* block = buffer->head;
	while (nread <  nsize) {
		if (is_empty_block(block))
			break;

		nread += read_from_block(block, pdata + nread, nsize - nread);
		buffer->nsize -= nread;

		st_block* o_block = block;
		block = block->next;

		if (is_empty_block(o_block)) {
			buffer->tail->next = o_block;
			buffer->tail = o_block;
			o_block->next = NULL;
			buffer->head = block;
			if (o_block == buffer->free) {
				buffer->free = buffer->head;
			}
		}
	}
	return nread;
}

int print_buffer(st_buffer* buffer) {
	st_block* block = buffer->head;
	while (block) {
		printf("block:%x begin:%d end:%d  size:%d  next:%x data:%d\n", block, block->begin, block->end, block->size, block->next, block->data);

		block = block->next;
	}
	return 0;
}


#endif
