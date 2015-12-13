#include "kv.h"

typedef struct {
  char *key;
  char *val;
} KeyVal;

KeyVal *KeyVal_new(char *key, char *val)
{
  KeyVal *kv = calloc(1, sizeof(KeyVal));

  if(kv != NULL)
  {
    kv->key = strdup(key);
    kv->val = strdup(val);
  }

  return kv;
}

void KeyVal_destroy(KeyVal *kv)
{
  if(kv != NULL)
  {
    free(kv->key);
    free(kv->val);
  }
  free(kv);
}
