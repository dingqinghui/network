
#ifndef __ERROR_H__
#define __ERROR_H__

#include <errno.h>
#include <stdio.h>
#include "../include/netlogger.h"

#define NET_RET_NULL 0

#define NET_RET_ERROR  -1
#define NET_RET_OK      0

#define NET_ERR_LEN 256


#define CHECK_PTR_ERR(ptr)  \
if(ptr == 0) \
{ \
    NET_LOG_ERROR("ptr is null");\
    return NET_RET_ERROR; \
} 

#define CHECK_PTR_RET_NULL(ptr)  \
if(ptr == 0) \
{ \
    NET_LOG_ERROR("ptr is null");\
    return NET_RET_NULL; \
} 

#define ERR_STR   strerror(errno)

void setError(char *err, const char *fmt, ...);

#endif

