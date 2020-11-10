#ifndef __LOGGER_H__
#define __LOGGER_H__


#define NLOG_LVL_DEBUG  0
#define NLOG_LVL_INFO   1
#define NLOG_LVL_WARN   2
#define NLOG_LVL_ERROR  3
#define NLOG_LVL_MAX  4



#define LOG_DEBUG(log,format,...) nlog_format(log,NLOG_LVL_DEBUG,__FUNCTION__,__LINE__,format,__VA_ARGS__)
#define LOG_INFO(log,format,...) nlog_format(log,NLOG_LVL_INFO,__FUNCTION__,__LINE__,format,__VA_ARGS__)
#define LOG_WARN(log,format,...) nlog_format(log,NLOG_LVL_WARN,__FUNCTION__,__LINE__,format,__VA_ARGS__)
#define LOG_ERROR(log,format,...) nlog_format(log,NLOG_LVL_ERROR,__FUNCTION__,__LINE__,format,__VA_ARGS__)


struct nlog;

struct nlog* nlog_init(const char* path,int level, long long max_file_size);
void nlog_format2(struct nlog* log,int level,char* format,...);
void nlog_format(struct nlog* log,int level,const char* funname,int lineno,char* format,...);
void nlog_set_level(struct nlog* log,int level);
void nlog_free(struct nlog* log);

#endif