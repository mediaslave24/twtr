#include "twitter_oauth_header.h"
#include "util.h"

static void strtoupper(char *str, unsigned int len)
{
  for(int i = 0; i < len && str[i] != '\0'; i++)
    str[i] = toupper(str[i]);
}

static char *gen_signature_base(const char *http_method, const char *url, const char *params)
{
  char *_url = NULL,
       *_params = NULL,
       *_http_method = NULL,
       *buf = NULL;
  int size = 0;

  _url = oauth_url_escape(url);
  _params = oauth_url_escape(params);
  _http_method = strdup(http_method);
  if(_url == NULL || _params == NULL || _http_method  == NULL)
    return NULL;

  strtoupper(_http_method, strlen(_http_method));

  size = strlen(_url) + strlen(_params) + strlen(_http_method) + 3;
  buf = calloc(1, size);
  if(buf == NULL)
    return NULL;
  log_debug("Generating sugnature_base from http_method: \"%s\", url: \"%s\", params: \"%s\"",
      _http_method, _url, _params);
  snprintf(buf, size, "%s&%s&%s", _http_method, _url, _params);

  free(_url);
  free(_params);
  free(_http_method);

  return buf;
}

static char *gen_signature_key(const char *consumer_secret, const char *token_secret)
{
  int size = strlen(consumer_secret) + strlen(token_secret) + 2;
  char *key = calloc(1, size);
  log_debug("Generating signature key from consumer_secret: \"%s\", token_secret: \"%s\"",
      consumer_secret, token_secret);
  snprintf(key, size, "%s&%s", consumer_secret, token_secret);
  return key;
}

char *twitter_oauth_header(const char *consumer_key, const char *consumer_secret,
    const char *http_method, const char *url, const char *params,
    const char *oauth_callback, const char *oauth_token, const char *token_secret)
{
  char *p = NULL;

  int bufsize = 256;
  char *buf = calloc(1, bufsize);

  /* Signature */
  char *signature_base = gen_signature_base(http_method, url, params);
  char *signature_key = gen_signature_key(consumer_secret, token_secret);
  p = oauth_sign_hmac_sha1(signature_base, signature_key);
  log_debug("Generating signature from base: \"%s\", key: \"%s\"", signature_base, signature_key);
  char *signature = oauth_url_escape(p);
  free(p);
  free(signature_base);
  free(signature_key);

  /* Nonce */
  char *nonce = oauth_gen_nonce();

  /* Timestamp */
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
      buf = realloc(buf, (bufsize += 128));
    }
  free(signature);
  free(nonce);

  return buf;
}
