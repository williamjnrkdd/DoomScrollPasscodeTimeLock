#include "logger.h"
#include <string.h>
#include <stdio.h>

void log_impl( char *message, char* file, int line, ...){
    va_list args;
    va_start(args, line);
    printf("[%s:%d] ", file, line);
    vprintf(message, args);
}