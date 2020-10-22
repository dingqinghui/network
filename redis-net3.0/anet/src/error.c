
#include "../include/error.h"
#include <stdio.h>
#include <stdarg.h>


void setError(char *err, const char *fmt, ...)
{
    va_list ap;
    if (!err) return;
    va_start(ap, fmt);
    vsnprintf(err, NET_ERR_LEN, fmt, ap);
    va_end(ap);
}
