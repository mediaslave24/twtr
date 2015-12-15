#include <string.h>
#include <curl.h>
#include <stdlib.h>

typedef enum { GET, POST } HTTP_METHOD;
void httprequest_init();
void httprequest_cleanup();
char *httprequest(HTTP_METHOD m, char *url, long include_header, char **headers);
