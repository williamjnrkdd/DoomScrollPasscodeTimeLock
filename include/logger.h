#include <stdarg.h>

#define log(x, ...) log_impl(x, __FILE__, __LINE__, ##__VA_ARGS__)

void log_impl(char *message, char* file, int line, ...);
