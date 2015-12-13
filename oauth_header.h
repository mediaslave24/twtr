#include <time.h>
#include <oauth.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kv.h"

#define OA_HMAC_SHA1_NAME "HMAC-SHA1"
#define OA_RSA_NAME "RSA"
#define OA_PLAINTEXT_NAME "PLAINTEXT"
#define OH_BUFSIZE 128

typedef struct _oauth_header {
  KeyVal **kvs;
} oauth_header;

oauth_header *oauth_header_new(
    OAuthMethod signature_method,
    const char * consumer_key,
    const char * signature_base,
    const char * signature_key
);

void oauth_header_destroy(oauth_header *h);
char * oauth_header_tostr(oauth_header *h);
char * oauth_header_smtostr(OAuthMethod m);
