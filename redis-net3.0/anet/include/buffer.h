#ifndef __BUFFER_H__
#define __BUFFER_H__

typedef struct buffer
{
    char* data;
    int capMax;
    int readIndex;
    int writeIndex;
	int curSize;
}buffer;


buffer* bufferCreate(int initSize);
int bufferFree(buffer* pBuf);

int bufferIsEmpty(buffer* pBuf);
int bufferIsFull(buffer* pBuf);

int bufferWrite(buffer* pBuf,char* buf, int size);
int bufferRead(buffer* pBuf,char* buf, int size);
int bufferReadAll(buffer* pBuf, char* buf);


int bufferPrint(buffer* pBuf);
#endif