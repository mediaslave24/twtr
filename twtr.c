#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <oauth.h>
#include <ctype.h>
#include <curl.h>
#include "util.h"
#include "twitter_oauth_header.h"

void usage(const char *basename)
{
  printf(
    "Usage: %s -s consumer-secret -k consumer-key\n"
  , basename);
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
  }

  return status;
}
