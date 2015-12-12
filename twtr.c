#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <oauth.h>
#include <ctype.h>
#include <time.h>
#include <bstrlib.h>

#define CONSUMER_KEY_SIZE 128
#define NONCE_SIZE 32
#define CHECK(p) if(!(p)) goto error

char *consumer_secret = "shit";
char *consumer_key = "my_consumer_key";
OAuthMethod method = OA_HMAC;

void die(char *msg)
{
  printf(msg);
  exit(1);
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		perror(NULL);
	return p;
}

bstring oauth_signature_method_bstr(OAuthMethod m)
{
  bstring name;
  switch(m) {
    case OA_HMAC:
      name = bfromcstr("HMAC-SHA1");
      break;
    case OA_RSA:
      name = bfromcstr("RSA-SHA1");
      break;
    case OA_PLAINTEXT:
      name = bfromcstr("PLAINTEXT");
      break;
    default:
      die("Unknown signature method\n");
  }
  CHECK(name != NULL);
  return name;
error:
  return NULL;
}

typedef struct _oauth_header {
  bstring consumer_key;
  bstring nonce;
  bstring signature;
  OAuthMethod signature_method;
  time_t timestamp;
} oauth_header;

oauth_header *oauth_header_new(
    OAuthMethod signature_method,
    bstring consumer_key,
    bstring signature_base,
    bstring signature_key
    )
{
char *(*sign)(const char *, const char *);
char *p;
  oauth_header *h = calloc(1, sizeof(oauth_header));
  CHECK(h != NULL);
  h->consumer_key = consumer_key;
  CHECK((p = oauth_gen_nonce()) != NULL);
  CHECK((h->nonce = bfromcstr(p)) != NULL);
  CHECK(h->nonce != NULL);
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
      die("Unknown signature method\n");
  }
  h->signature_method = signature_method;

  p = sign((const char *) signature_base->data, (const char *) signature_key->data);
  CHECK(p != NULL);
  h->signature = bfromcstr(p);
  CHECK(h->signature != NULL);
  free(p);
  ctime(&h->timestamp);
  return h;

error:
  return NULL;
}

void oauth_header_destroy(oauth_header *h)
{
  bdestroy(h->signature);
  bdestroy(h->nonce);
  bdestroy(h->consumer_key);
  free(h);
}

bstring oauth_header_bstr(oauth_header *h)
{
  bstring str;
  bstring signature_method = oauth_signature_method_bstr(h->signature_method);
  CHECK(signature_method != NULL);

  str = bformat(
      "oauth_consumer_key=\"%s\"\n"
      "oauth_nonce=\"%s\"\n"
      "oauth_signature=\"%s\"\n"
      "oauth_signature_method=\"%s\"\n"
      "oauth_timestamp=\"%u\"\n"
      "oauth_version=\"1.0\"\n"
      , h->consumer_key->data
      , h->nonce->data
      , h->signature->data
      , signature_method->data
      , h->timestamp);

  return str;

error:
  return NULL;
}

void usage(const char *basename)
{
  printf(
    "Usage: %s\n"
    "Program for reading twitter\n"
  , basename);
}

int main(int argc, const char *argv[])
{
  for(int i = 1; i < argc; i++)
    if(!(strcmp(argv[i], "-h")))
      usage(argv[0]);
  oauth_header *h = oauth_header_new(
      OA_HMAC,
      bfromcstr(consumer_key),
      bfromcstr("base"),
      bfromcstr("key")
      );
  printf("%s\n", oauth_header_bstr(h)->data);
  return 0;
}
