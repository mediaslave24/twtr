#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
#define log_debug(F, ...) fprintf(stdout, "%s:%d "F" \n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define log_debug(F, ...)
#endif

void die(char *msg);

void * ecalloc(size_t nmemb, size_t size);
