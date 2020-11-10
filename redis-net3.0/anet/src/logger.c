
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
 #include <stdarg.h>
#include <time.h>
#include <string.h>
#include <errno.h>
 //#include <pthread.h>
 #include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "../include/logger.h"
#include "../include/zmemory.h"

#define FILE_PATH_MAX 5000
#define FILE_NAME_MAX 255

#define MALLOC zmalloc
#define BUFFER_SIZE 65535


static char* levelName[] = {
    [NLOG_LVL_DEBUG] = "DEBUG",
    [NLOG_LVL_INFO] = "INFO",
    [NLOG_LVL_WARN] = "WARN",
    [NLOG_LVL_ERROR] = "ERROR",
};


typedef struct nlog_writer{
    char filepath[FILE_PATH_MAX];        //文件路径
    int rollindex;                      //轮存索引

    long long  written_size;             //写入大小
    long long  max_file_size;           //单文件最大大小

    FILE * file;                        //文件流
    int level;                          //日志级别

    struct tm vtm; 
}nlog_writer;


void log_writer_init(nlog_writer* writer ,const char* path,int level,long long  max_file_size){
    writer->rollindex = 0;
    writer->written_size = 0;
    writer->max_file_size = max_file_size;
    writer->level = level;
    writer->file = 0;

    memcpy(writer->filepath,path,sizeof(path));
}

void log_writer_create_file(nlog_writer* writer){
 
    char filepath[FILE_PATH_MAX];
    time_t now;
	now = time(&now);;
	struct tm vtm; 
    localtime_r(&now, &vtm);
    sprintf(filepath,"%s/%d-%02d-%02d/",writer->filepath, 
            vtm.tm_year+ 1900,vtm.tm_mon + 1, vtm.tm_mday);

    //创建目录
    char commond[FILE_PATH_MAX + 20];
    sprintf(commond,"mkdir -p %s",filepath);
    if( system(commond) < 0){
        printf("mkdir err.\n");
        return;
    }

    char fullpath[FILE_PATH_MAX];
    sprintf(fullpath,"%s%s_%02d%02d%02d_%d",filepath,levelName[writer->level],
            vtm.tm_hour, vtm.tm_min, vtm.tm_sec,
            writer->rollindex++);

    //打开文件
    writer->file =  fopen(fullpath,"w+");
    if(writer->file == 0){
        printf("fopen err.err:%s\n",strerror(errno));
        return ;
    }
    memcpy(&(writer->vtm),&(vtm),sizeof(vtm));

    // int fd = fileno(writer->file);
    // int flag = fcntl(fd, F_GETFL, 0);
    // if( fcntl(fd, F_SETFL, flag | O_NONBLOCK) < 0){
    //      printf("fcntl O_NONBLOCK err.err:%s\n",strerror(errno));
    //      return ;
    // }
}

void log_writer_tofile(nlog_writer* writer,char* str,int size){
    time_t now;
	now = time(&now);;
	struct tm vtm; 
    localtime_r(&now, &vtm);

    int newday = vtm.tm_year != writer->vtm.tm_year || 
                vtm.tm_mon != writer->vtm.tm_mon ||
                vtm.tm_mday != writer->vtm.tm_mday;
    int roll = writer->written_size >= writer->max_file_size;
    //检测跨天/超过单文件大小上限
    if(newday || roll){
        if(writer->file){
            fclose(writer->file);
            writer->file = 0;
            writer->written_size = 0;
        }
    }
    if(!writer->file){
        log_writer_create_file(writer);
    }
   
    //写入数据
    if( 1 != fwrite(str,size,1,writer->file) ){
        printf("fwrite err.err:%s\n",strerror(errno));
    }

    writer->written_size += size;
    // 这两行对性能影响较大
    // int fd = fileno(writer->file);
    // fflush(writer->file);  //用户态切换到内核态，库缓存->内核缓存
    // fsync(fd);             //内核缓存->磁盘 太慢了 
}


void log_writer_free(nlog_writer* writer){
    if (writer){
        if(writer->file){
            fclose(writer->file);
        }
        free(writer);
    }
}


typedef struct nlog{
    int level;
    long long  max_file_size;
    char filepath[FILE_PATH_MAX];        
    nlog_writer* writerArray[NLOG_LVL_MAX];
    char* buff[BUFFER_SIZE];
} nlog;


nlog* nlog_init(const char* path,int level, long long  max_file_size){
    nlog* log = MALLOC(sizeof(nlog));
    assert(log);

    log->level =  level;
    log->max_file_size =  max_file_size;
    memcpy(log->filepath,path,sizeof(path));

    for(int lv = 0;lv < NLOG_LVL_MAX;++lv){
        log->writerArray[lv] = 0;
    }
    return log;
}

static int on_format_callinfo(char* buf,int bufsize,const char* funname,int lineno){
    return snprintf(buf,bufsize,"[%s][%d]",funname,lineno);
}

//[DEBUG]:[2020-11-10 10:21:26]
static int on_format_time_info(char* buffer,int bufsize, int l)
{
    time_t now;
	now = time(&now);;
	struct tm vtm; 
    localtime_r(&now, &vtm);
    return snprintf(buffer, bufsize, "[%s]:[%d-%02d-%02d %02d:%02d:%02d]", levelName[l],
            vtm.tm_year+ 1900,vtm.tm_mon + 1, vtm.tm_mday, vtm.tm_hour, vtm.tm_min, vtm.tm_sec);
}


void nlog_format(struct nlog* log,int level,const char* funname,int lineno,char* format,...){
    assert(log);

    if(level < log->level){
        return ;
    }

    char* buf = log->buff;
    //行首格式化
    int time_size = on_format_time_info(buf,BUFFER_SIZE,level);
    buf += time_size;

    int call_size = on_format_callinfo(buf,BUFFER_SIZE - time_size,funname,lineno);
    buf += call_size;
    

    //主体格式化
    va_list args;
    va_start(args, format);
    int body_size = vsnprintf(buf, BUFFER_SIZE - time_size - call_size, format, args);
    va_end(args);

    //获取writer
    nlog_writer* writer =  log->writerArray[level];
    if(!writer){
        writer = MALLOC(sizeof(nlog_writer));
        log->writerArray[level] = writer;
        log_writer_init( writer , log->filepath, level, log->max_file_size);
    }
    //写入
    log_writer_tofile(writer,log->buff,body_size + time_size + call_size);
}

void nlog_format2(nlog* log,int level,char* format,...){
    assert(log);

    if(level < log->level){
        return ;
    }

    char* buf = log->buff;
    //行首格式化
    int time_size = on_format_time_info(buf,BUFFER_SIZE,level);
    buf += time_size;
    //主体格式化
    va_list args;
    va_start(args, format);
    int body_size = vsnprintf(buf, BUFFER_SIZE - time_size, format, args);
    va_end(args);

    //获取writer
    nlog_writer* writer =  log->writerArray[level];
    if(!writer){
        writer = MALLOC(sizeof(nlog_writer));
        log->writerArray[level] = writer;
        log_writer_init( writer , log->filepath, level, log->max_file_size);
    }
    //写入
    log_writer_tofile(writer,log->buff,body_size + time_size);
}


void nlog_set_level(nlog* log,int level){
    assert(log);
    log->level = level;
}


void nlog_free(nlog* log){
    if(log){
        for(int lv = 0;lv < NLOG_LVL_MAX;++lv){
            nlog_writer* writer = log->writerArray[lv] ;
            log_writer_free(writer);
        }
        free(log);
    }
}