#include <string.h>
#include <stdlib.h>

typedef struct {
  char *key;
  char *val;
} KeyVal;

KeyVal *KeyVal_new(char *key, char *val);
void KeyVal_destroy(KeyVal *kv);
