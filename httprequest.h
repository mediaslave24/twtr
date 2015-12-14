#include <string.h>
#include <curl.h>
#include <stdlib.h>

typedef enum { GET, POST } HTTP_METHOD;

static struct buf { char *data; char *end; size_t size; };

void httprequest_init()
{
  curl_global_init(CURL_GLOBAL_ALL);
}

void httprequest_cleanup()
{
  curl_global_cleanup();
}

static size_t write(char *p, size_t n, size_t s, void *out)
{
  size_t size = n * s;
  size_t offset = 0;
  struct buf *buf = (struct buf *)out;

  if(buf->data == NULL)
  {
    buf->size = BUFSIZ;
    buf->data = calloc(1, buf->size);
    buf->end = buf->data;
  }

  while((buf->size - (offset = buf->end - buf->data)) < size)
  {
    buf->size += BUFSIZ;
    buf->data = realloc(buf->data, buf->size);
    buf->end = buf->data + offset;
  }
  memcpy(buf->end, p, size);
  buf->end += size;

  return size;
}

char *httprequest(HTTP_METHOD m, char *url, long include_header, char **headers)
{
  CURL *curl = curl_easy_init();
  struct buf buf = { .size = 0, .data = NULL, end = NULL };
  struct curl_slist *hlist = NULL;

  /* Set URL */
  curl_easy_setopt(curl, CURLOPT_URL, url);

  /* Set HTTP method */
  if(m == GET)
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  else if(m == POST)
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
  else
    return NULL;

  /* Set to include http header in output */
  curl_easy_setopt(curl, CURLOPT_HEADER, include_header);

  /* Set headers */
  if(headers != NULL)
  {
    for(int i = 0; headers[i] != NULL; i++)
      curl_slist_append(list, headers[i]);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
  }

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

  curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  if(headers != NULL)
    curl_slist_free_all(hlist);
}
