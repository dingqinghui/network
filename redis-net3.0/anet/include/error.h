
#ifndef __ERROR_H__
#define __ERROR_H__

#define NET_RET_NULL 0

#define NET_RET_ERROR  -1
#define NET_RET_OK      0

#define NET_ERR_LEN 256


#define PRINT_ERR(msg) \
printf("%s.function:%s file:%s line:%d\n",msg,__FUNCTION__,__FILE__,__LINE__);

#define CHECK_PTR_ERR(ptr)  \
if(ptr == 0) \
{ \
    PRINT_ERR("")\
    return NET_RET_ERROR; \
} 

#define CHECK_PTR_RET_NULL(ptr)  \
if(ptr == 0) \
{ \
    PRINT_ERR("")\
    return NET_RET_NULL; \
} 



void setError(char *err, const char *fmt, ...);

#endif

