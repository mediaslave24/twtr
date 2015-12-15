#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <oauth.h>
#include <ctype.h>

char *twitter_oauth_header(const char *consumer_key, const char *consumer_secret,
    const char *http_method, const char *url, const char *params,
    const char *oauth_callback, const char *oauth_token, const char *token_secret);
