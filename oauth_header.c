#include "oauth_header.h"

char *oauth_header_smtostr(OAuthMethod m)
{
  char *str = NULL;
  switch(m) {
    case OA_HMAC:
      str = calloc(1, sizeof(OA_HMAC_SHA1_NAME));
      strcpy(str, OA_HMAC_SHA1_NAME);
      break;
    case OA_RSA:
      str = calloc(1, sizeof(OA_RSA_NAME));
      strcpy(str, OA_RSA_NAME);
      break;
    case OA_PLAINTEXT:
      str = calloc(1, sizeof(OA_PLAINTEXT_NAME));
      strcpy(str, OA_PLAINTEXT_NAME);
      break;
  }
  return str;
}

oauth_header *oauth_header_new(
    OAuthMethod signature_method,
    const char *consumer_key,
    const char *signature_base,
    const char *signature_key
    )
{
char *p = NULL;
char *(*sign)(const char *, const char *);

  oauth_header *h = calloc(1, sizeof(oauth_header));
  if(h == NULL) return NULL;

  h->consumer_key = oauth_url_escape(consumer_key);
  p = oauth_gen_nonce();
  h->nonce = oauth_url_escape(p);
  free(p);

  switch(h->signature_method) {
    case OA_HMAC:
      sign = oauth_sign_hmac_sha1;
      break;
    case OA_PLAINTEXT:
      sign = oauth_sign_plaintext;
      break;
    case OA_RSA:
      sign = oauth_sign_rsa_sha1;
      break;
    default:
      return NULL;
  }
  h->signature_method = signature_method;
  p = sign((const char *) signature_base, (const char *) signature_key);
  h->signature = oauth_url_escape(p);
  free(p);
  time(&h->timestamp);

  if(h->consumer_key == NULL ||
      h->signature == NULL ||
      h->nonce == NULL)
    return NULL;

  return h;
}

void oauth_header_destroy(oauth_header *h)
{
  free(h->signature);
  free(h->nonce);
  free(h->consumer_key);
  free(h);
}

char *oauth_header_tostr(oauth_header *h)
{
  size_t size = OH_BUFSIZE;
  char *buf = calloc(1, size);
  char *signature_method = oauth_header_smtostr(h->signature_method);
  char *fmt =
    "Authorization: OAuth oauth_consumer_key=\"%s\", oauth_nonce=\"%s\", oauth_signature=\"%s\", "
    "oauth_signature_method=\"%s\", oauth_timestamp=\"%u\", oauth_version=\"1.0\"";

  while((snprintf(buf, size, fmt, h->consumer_key,
                  h->nonce, h->signature, signature_method,
                  h->timestamp) + 1) > size)
  { size += OH_BUFSIZE; buf = realloc(buf, size); }
  return buf;
}
