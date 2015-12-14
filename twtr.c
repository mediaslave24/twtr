#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <oauth.h>
#include <ctype.h>
#include <curl.h>
#include "util.h"
#include "oauth_header.h"
void usage(const char *basename)
{
  printf(
    "Usage: %s -s consumer-secret -k consumer-key\n"
  , basename);
}

static void strtoupper(char *str, unsigned int len)
{
  for(int i = 0; i < len && str[i] != '\0'; i++)
    str[i] = toupper(str[i]);
}

static struct buf {
  char *data;
  char *end;
  size_t size;
};

static size_t get_response(char *p, size_t size, size_t n, void *resp)
{
  struct buf *b = (struct buf*) resp;
  size_t s = size * n;
  buf_append(b, p, s);
  return s;
}

char *twitter_oauth_header(const char *consumer_key, const char *consumer_secret,
    const char *http_method, const char *url, const char *params,
    const char *oauth_callback, const char *oauth_token, const char *token_secret)
{
char *p;

  int bufsize = 256;
  char *buf = calloc(1, bufsize);

  char *_url = oauth_url_escape(url);
  char *_params = oauth_url_escape(params);
  char *_http_method = strdup(http_method);
  strtoupper(_http_method, strlen(_http_method));

  int signature_base_len = strlen(_url) + strlen(_params) + strlen(_http_method) + 3;
  char *signature_base = calloc(1, signature_base_len);
  int signature_key_len = strlen(consumer_secret) + strlen(token_secret) + 2;
  char *signature_key = calloc(1, signature_key_len);

  snprintf(signature_base, signature_base_len, "%s&%s&%s", _http_method, _url, _params);
  snprintf(signature_key, signature_key_len, "%s&%s", consumer_secret, token_secret);

  p = oauth_sign_hmac_sha1(signature_base, signature_key);
  char *signature = oauth_url_escape(p);
  free(p);
  char *nonce = oauth_gen_nonce();
  time_t timestamp = time(NULL);

  if(strlen(oauth_token) > 0)
    while((snprintf(buf, bufsize,
          "Authorization: OAuth "
          "oauth_callback=\"%s\", "
          "oauth_consumer_key=\"%s\", "
          "oauth_nonce=\"%s\", "
          "oauth_signature=\"%s\", "
          "oauth_signature_method=\"HMAC-SHA1\", "
          "oauth_timestamp=\"%ld\", "
          "oauth_token=\"%s\", "
          "oauth_version=\"1.0\""
          , oauth_callback, consumer_key, nonce, signature, timestamp, token_secret)) >= (bufsize-1))
    {
      buf = realloc(buf, (bufsize += 128));
    }
  else
    while((snprintf(buf, bufsize,
         "Authorization: OAuth "
          "oauth_callback=\"%s\", "
          "oauth_consumer_key=\"%s\", "
          "oauth_nonce=\"%s\", "
          "oauth_signature=\"%s\", "
          "oauth_signature_method=\"HMAC-SHA1\", "
          "oauth_timestamp=\"%ld\", "
          "oauth_version=\"1.0\""
          , oauth_callback, consumer_key, nonce, signature, timestamp)) >= (bufsize-1))
    {
      printf("Realloc\n");
      buf = realloc(buf, (bufsize += 128));
    }
  free(_url);
  free(_params);
  free(_http_method);
  free(signature_base);
  free(signature_key);
  free(signature);
  free(nonce);

  return buf;
}

int main(int argc, const char *argv[])
{

int status = 0;
const char *consumer_key = NULL;
const char *consumer_secret = NULL;

  for(int i = 1; i < argc; i++)
    if(!(strcmp(argv[i], "-h")))
      usage(argv[0]);
    else if(!(strcmp(argv[i], "-k")))
      consumer_key = argv[++i];
    else if(!(strcmp(argv[i], "-s")))
      consumer_secret = argv[++i];

  if(consumer_key == NULL)
  {
    status = 1;
    printf("Consumer key not specified\n");
    usage(argv[0]);
  } else if(consumer_secret == NULL)
  {
    status = 1;
    printf("Consumer secret not specified\n");
    usage(argv[0]);
  }
  else
  {
    curl_global_init(CURL_GLOBAL_ALL);

    const char *http_method = "POST";
    const char *url = "https://api.twitter.com/oauth/request_token";
    const char *params = "";
    char *h = twitter_oauth_header(
          consumer_key, consumer_secret, http_method, url, params, "oob", "", "");
    struct buf *b = buf_new();
    struct curl_slist *headers = NULL;

    curl_slist_append(headers, h);

    CURL *curl = curl_easy_init();

    if(curl == NULL)
      die("Can't init curl\n");

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, b);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_response);

    curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    free(h);
    curl_slist_free_all(headers);
    curl_global_cleanup();
  }

  return status;
}
