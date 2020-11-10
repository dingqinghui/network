#ifndef __LOGGER_H__
#define __LOGGER_H__


#define NLOG_LVL_DEBUG  0
#define NLOG_LVL_INFO   1
#define NLOG_LVL_WARN   2
#define NLOG_LVL_ERROR  3
#define NLOG_LVL_MAX  4
/*
    1. #define debug(format, ...) fprintf (stderr, format,  __VA_ARGS__) 
        不允许可变参数列表未空debug("11111") 将报错
    2.  #define debug(format, ...) fprintf (stderr, format, ## __VA_ARGS__)   
        允许可变参数列表未空。这里，如果可变参数被忽略或为空，'##'操作将使预处理器(preprocessor)去除掉它前面的那个逗号。如果你在宏调用时，确实提供了一些可变参数，
        GNU CPP也会工作正常，它会把这些可变参数放到逗号的后面。象其它的pasted macro参数一样，这些参数不是宏的扩展。
        ##还可以起到替换作用
        如:#define FUN(IName)  IName##_ptr

*/

#define LOG_DEBUG(log,format,...) nlog_format(log,NLOG_LVL_DEBUG,__FUNCTION__,__LINE__,format,##__VA_ARGS__)
#define LOG_INFO(log,format,...) nlog_format(log,NLOG_LVL_INFO,__FUNCTION__,__LINE__,format,##__VA_ARGS__)
#define LOG_WARN(log,format,...) nlog_format(log,NLOG_LVL_WARN,__FUNCTION__,__LINE__,format,##__VA_ARGS__)
#define LOG_ERROR(log,format,...) nlog_format(log,NLOG_LVL_ERROR,__FUNCTION__,__LINE__,format,##__VA_ARGS__)


struct nlog;

struct nlog* nlog_init(const char* path,int level, long long max_file_size);
void nlog_format2(struct nlog* log,int level,char* format,...);
void nlog_format(struct nlog* log,int level,const char* funname,int lineno,char* format,...);
void nlog_set_level(struct nlog* log,int level);
void nlog_free(struct nlog* log);

#endif