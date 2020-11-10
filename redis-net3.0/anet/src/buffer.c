

#include <string.h>
#include <assert.h>

#include "../include/zmemory.h"
#include "../include/error.h"
#include "../include/buffer.h"
#include "../include/netlogger.h"

#define APPEND_CAP_SCALE 2

buffer* bufferCreate(int initSize){
	assert(initSize > 0);
    buffer* buf = zmalloc(sizeof(buffer));

	buf->data = zmalloc(sizeof(char) * initSize);
	buf->capMax = initSize;
	buf->readIndex = 0;
	buf->writeIndex = 0;
	buf->curSize = 0;

    return buf;
}
int bufferFree(buffer* pBuf) {
	CHECK_PTR_ERR(pBuf)
	CHECK_PTR_ERR(pBuf->data);
	zfree(pBuf->data);
	zfree(pBuf);
}

int bufferIsEmpty(buffer* pBuf) {
	return pBuf->curSize == 0;
}
int bufferIsFull(buffer* pBuf) {
	return pBuf->curSize == pBuf->capMax;
}

int bufferExpand(buffer* pBuf,int size) {
	CHECK_PTR_ERR(pBuf)
	int cap = (pBuf->curSize + size) * APPEND_CAP_SCALE;
	char* ptr = zmalloc(sizeof(char) * cap);

	int oldCap = pBuf->curSize;

	bufferRead(pBuf, ptr, pBuf->curSize);

	zfree(pBuf->data);
	pBuf->data = ptr;
	pBuf->readIndex = 0;
	pBuf->writeIndex = oldCap;
	pBuf->capMax = cap;
	pBuf->curSize = oldCap;

	return NET_RET_OK;
}

int bufferWrite(buffer* pBuf, char* buf, int size) {
	if (!pBuf) {
		return 0;
	}
	if (size <= 0) {
		return 0;
	}
	int remain = pBuf->capMax - pBuf->curSize;
	if (remain < size) {
		if (bufferExpand(pBuf,size) == NET_RET_ERROR)
			return 0;
	}
	if (pBuf->writeIndex + size > pBuf->capMax) {
		int n = pBuf->capMax - pBuf->writeIndex;
		memcpy(pBuf->data + pBuf->writeIndex, buf, n);
		memcpy(pBuf->data , buf + n, size - n );
		pBuf->writeIndex = size - n;
	}
	else {
		memcpy(pBuf->data + pBuf->writeIndex, buf, size);

		pBuf->writeIndex += size;
	}
	if (pBuf->writeIndex >= pBuf->capMax) {
		pBuf->writeIndex -= pBuf->capMax;
	}
	pBuf->curSize += size;
	//bufferPrint(pBuf);
	return size;
}



int bufferReadAll(buffer* pBuf, char* buf) {
	return bufferRead(pBuf, buf, pBuf->curSize);
}

int bufferRead(buffer* pBuf, char* buf, int size) {
	if (!pBuf) {
		return 0;
	}
	if (size <= 0) {
		return 0;
	}
	if (bufferIsEmpty(pBuf)) {
		return 0;
	}
	int maxCopy = pBuf->curSize > size ? size : pBuf->curSize;
	if (pBuf->readIndex > pBuf->writeIndex) {
		int n = pBuf->capMax - pBuf->readIndex;
		if ( n < maxCopy) {
			memcpy(buf, pBuf->data + pBuf->readIndex, n);
			memcpy(buf + n, pBuf->data, maxCopy - n);
			pBuf->readIndex = maxCopy - n;
		}
		else {
			memcpy(buf, pBuf->data + pBuf->readIndex, maxCopy);
			pBuf->readIndex += maxCopy;
		}
	}
	else {
		memcpy(buf, pBuf->data + pBuf->readIndex, maxCopy);
		pBuf->readIndex += maxCopy;
	}
	if (pBuf->readIndex >= pBuf->capMax) {
		pBuf->readIndex -= pBuf->capMax;
	}
	pBuf->curSize -= maxCopy;
	//bufferPrint(pBuf);
	return maxCopy;
}

int bufferPrint(buffer* pBuf) {
	NET_LOG_DEBUG("cap:%d cur:%d readIndex:%d writeIndex:%d \n", pBuf->capMax, pBuf->curSize, pBuf->readIndex, pBuf->writeIndex);
}
