
#ifndef __NET_LOGGER_H__
#define __NET_LOGGER_H__

#include "../include/logger.h"


#define NET_LOG_DEBUG(format,...)  LOG_DEBUG(netloger_ins(),format,##__VA_ARGS__)
#define NET_LOG_INFO(format,...)  LOG_INFO(netloger_ins(),format,##__VA_ARGS__)
#define NET_LOG_WARN(format,...)  LOG_WARN(netloger_ins(),format,##__VA_ARGS__)
#define NET_LOG_ERROR(format,...)  LOG_ERROR(netloger_ins(),format,##__VA_ARGS__)



struct nlog;

int netloger_init();
struct nlog* netloger_ins();


#endif


